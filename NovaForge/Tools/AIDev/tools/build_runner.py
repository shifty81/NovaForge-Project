"""
NovaForge Dev AI — Build Runner (Phase 0)

Runs CMake builds, test executables, and other project commands,
capturing stdout/stderr and returning structured results.
"""

import os
import subprocess
import logging
from pathlib import Path
from typing import Optional, Tuple

log = logging.getLogger(__name__)

# Default server build directory
_SERVER_BUILD_DIR = "cpp_server/build"


class BuildRunner:
    """
    Executes build commands for the NovaForge project and captures output.

    Supports:
      - Full server CMake build
      - Per-system test target builds
      - Running individual test binaries
      - Python / Lua script execution
      - Blender CLI invocation
      - Generic shell commands (with approval gate in the agent loop)
    """

    def __init__(self, repo_root: Path):
        self.repo_root = repo_root

    # ------------------------------------------------------------------
    # CMake / Server build
    # ------------------------------------------------------------------

    def configure_server(self, build_type: str = "Release") -> Tuple[str, bool]:
        """Run cmake configure step for the server."""
        build_dir = self.repo_root / _SERVER_BUILD_DIR
        build_dir.mkdir(parents=True, exist_ok=True)
        cmd = [
            "cmake", "..",
            f"-DCMAKE_BUILD_TYPE={build_type}",
            "-DBUILD_TESTS=ON",
            "-DUSE_STEAM_SDK=OFF",
        ]
        return self._run(cmd, cwd=build_dir)

    def build_server(self, target: Optional[str] = None, jobs: Optional[int] = None) -> Tuple[str, bool]:
        """
        Build the server (or a specific target).

        Args:
            target: CMake target name, e.g. 'test_fleet_debrief_system'.
                    None builds all.
            jobs:   Parallel jobs. Defaults to os.cpu_count().
        """
        build_dir = self.repo_root / _SERVER_BUILD_DIR
        if not (build_dir / "CMakeCache.txt").exists():
            out, ok = self.configure_server()
            if not ok:
                return out, False

        j = jobs or os.cpu_count() or 4
        cmd = ["cmake", "--build", ".", f"-j{j}"]
        if target:
            cmd += ["--target", target]
        return self._run(cmd, cwd=build_dir)

    def run_test(self, test_name: str) -> Tuple[str, bool]:
        """
        Run a compiled test binary.

        Args:
            test_name: Binary name without path, e.g. 'test_fleet_debrief_system'.
        """
        bin_path = self.repo_root / _SERVER_BUILD_DIR / "bin" / test_name
        if not bin_path.exists():
            # Try building it first
            out, ok = self.build_server(target=test_name)
            if not ok:
                return out, False
        return self._run([str(bin_path)], cwd=self.repo_root / _SERVER_BUILD_DIR)

    def run_all_tests(self) -> Tuple[str, bool]:
        """Run the monolithic test_systems binary."""
        return self.run_test("test_systems")

    # ------------------------------------------------------------------
    # Script runners
    # ------------------------------------------------------------------

    def run_python(self, rel_script: str, args: Optional[list] = None) -> Tuple[str, bool]:
        """Run a Python script relative to the repo root."""
        script = self.repo_root / rel_script
        if not script.exists():
            return f"Script not found: {rel_script}", False
        cmd = ["python", str(script)] + (args or [])
        return self._run(cmd, cwd=self.repo_root)

    def run_lua(self, rel_script: str, args: Optional[list] = None) -> Tuple[str, bool]:
        """Run a Lua script relative to the repo root."""
        script = self.repo_root / rel_script
        if not script.exists():
            return f"Script not found: {rel_script}", False
        cmd = ["lua", str(script)] + (args or [])
        return self._run(cmd, cwd=self.repo_root)

    def run_blender(self, rel_script: str, args: Optional[list] = None) -> Tuple[str, bool]:
        """
        Run a Blender Python script headlessly.
        Requires Blender to be on PATH.
        """
        script = self.repo_root / rel_script
        if not script.exists():
            return f"Blender script not found: {rel_script}", False
        cmd = ["blender", "--background", "--python", str(script)]
        if args:
            cmd += ["--"] + args
        return self._run(cmd, cwd=self.repo_root)

    def run_glsl_validate(self, rel_shader: str) -> Tuple[str, bool]:
        """Validate a GLSL shader with glslangValidator."""
        shader = self.repo_root / rel_shader
        if not shader.exists():
            return f"Shader not found: {rel_shader}", False
        return self._run(["glslangValidator", "-V", str(shader)], cwd=self.repo_root)

    # ------------------------------------------------------------------
    # C# / .NET build
    # ------------------------------------------------------------------

    def run_dotnet_build(self, rel_project: str = "",
                         configuration: str = "Debug") -> Tuple[str, bool]:
        """
        Build a .NET project or solution.

        Args:
            rel_project: Path relative to repo root (directory, .csproj, or .sln).
                         Empty string builds from repo root.
            configuration: Build configuration (Debug / Release).
        """
        cwd = self.repo_root / rel_project if rel_project else self.repo_root
        if cwd.is_file():
            cmd = ["dotnet", "build", str(cwd), "-c", configuration]
            cwd = cwd.parent
        else:
            cmd = ["dotnet", "build", "-c", configuration]
        return self._run(cmd, cwd=cwd)

    def run_dotnet_test(self, rel_project: str = "",
                        filter_expr: str = "") -> Tuple[str, bool]:
        """
        Run .NET tests via ``dotnet test``.

        Args:
            rel_project: Path to test project (directory or .csproj).
            filter_expr: Optional test filter expression (--filter).
        """
        cwd = self.repo_root / rel_project if rel_project else self.repo_root
        if cwd.is_file():
            cmd = ["dotnet", "test", str(cwd)]
            cwd = cwd.parent
        else:
            cmd = ["dotnet", "test"]
        if filter_expr:
            cmd += ["--filter", filter_expr]
        return self._run(cmd, cwd=cwd)

    # ------------------------------------------------------------------
    # GLSL validation (full workflow)
    # ------------------------------------------------------------------

    def validate_glsl_dir(self, rel_dir: str = "engine/shaders") -> Tuple[str, bool]:
        """
        Validate all GLSL shaders in a directory.

        Returns a combined report and overall success flag.
        """
        shader_dir = self.repo_root / rel_dir
        if not shader_dir.is_dir():
            return f"Shader directory not found: {rel_dir}", False

        results: list[str] = []
        all_ok = True
        exts = {".vert", ".frag", ".glsl", ".geom", ".tesc", ".tese", ".comp"}
        for path in sorted(shader_dir.rglob("*")):
            if path.suffix.lower() not in exts:
                continue
            rel = str(path.relative_to(self.repo_root))
            out, ok = self.run_glsl_validate(rel)
            if ok:
                results.append(f"  ✓ {rel}")
            else:
                results.append(f"  ✗ {rel}\n    {out}")
                all_ok = False

        if not results:
            return f"No shaders found in {rel_dir}", True

        header = f"GLSL validation ({len(results)} shader(s)):\n"
        return header + "\n".join(results), all_ok

    # ------------------------------------------------------------------
    # Generic command
    # ------------------------------------------------------------------

    def run_command(self, cmd: list, cwd: Optional[str] = None) -> Tuple[str, bool]:
        """
        Run an arbitrary shell command.
        NOTE: Should only be called after explicit user approval in the agent loop.
        """
        work_dir = Path(cwd) if cwd else self.repo_root
        return self._run(cmd, cwd=work_dir)

    # ------------------------------------------------------------------
    # Private
    # ------------------------------------------------------------------

    def _run(self, cmd: list, cwd: Path, timeout: int = 300) -> Tuple[str, bool]:
        """Execute a command and return (combined_output, success)."""
        log.info(f"Running: {' '.join(str(c) for c in cmd)}")
        try:
            result = subprocess.run(
                [str(c) for c in cmd],
                cwd=str(cwd),
                capture_output=True,
                text=True,
                timeout=timeout,
            )
            combined = ""
            if result.stdout:
                combined += result.stdout
            if result.stderr:
                combined += ("\n" if combined else "") + result.stderr
            success = result.returncode == 0
            if not success:
                log.warning(f"Command exited with code {result.returncode}")
            return combined.strip(), success
        except subprocess.TimeoutExpired:
            msg = f"Command timed out after {timeout}s"
            log.error(msg)
            return msg, False
        except FileNotFoundError as e:
            msg = f"Command not found: {cmd[0]} — {e}"
            log.error(msg)
            return msg, False
        except Exception as e:
            msg = f"Command failed: {e}"
            log.error(msg)
            return msg, False

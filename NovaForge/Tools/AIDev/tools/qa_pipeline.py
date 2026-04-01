"""
QA Pipeline — automated testing, build capture, and media validation.

Provides helpers for running cpp_server test binaries, generating ECS test
stubs, validating JSON schemas, and capturing cmake build output.
"""

import json
import os
import subprocess
from pathlib import Path


class QAPipeline:
    """Automated QA helpers for builds, tests, and asset validation."""

    # ------------------------------------------------------------------
    def run_unit_tests(self, test_binary_path, output_file=None):
        """Run a compiled test binary and return (success, output)."""
        try:
            result = subprocess.run(
                [test_binary_path], capture_output=True, text=True, timeout=120
            )
            out = result.stdout + result.stderr
            if output_file:
                Path(output_file).write_text(out, encoding="utf-8")
            return result.returncode == 0, out
        except Exception as e:
            return False, str(e)

    def run_all_server_tests(self, build_dir):
        """Run all cpp_server test binaries found in build_dir/bin/."""
        bin_dir = Path(build_dir) / "bin"
        results = {}
        if not bin_dir.exists():
            print(f"[QAPipeline] bin dir not found: {bin_dir}")
            return results
        for binary in sorted(bin_dir.glob("test_*")):
            if binary.is_file() and os.access(binary, os.X_OK):
                ok, out = self.run_unit_tests(str(binary))
                results[binary.name] = {"passed": ok, "lines": len(out.splitlines())}
        return results

    def generate_ecs_test_stub(self, system_name, component_name):
        """Return a cpp_server test file stub string for a new ECS system."""
        func = system_name[0].lower() + system_name[1:]  # camelCase
        return (
            f"// Tests for: {system_name}\n"
            f"#include \"test_log.h\"\n"
            f"#include \"ecs/world.h\"\n"
            f"#include \"systems/{_to_snake(system_name)}.h\"\n\n"
            f"using namespace atlas;\n\n"
            f"static void test{system_name}Init() {{\n"
            f"    ecs::World world;\n"
            f"    systems::{system_name} sys(&world);\n"
            f"    world.createEntity(\"e1\");\n"
            f"    assertTrue(sys.initialize(\"e1\"), \"init succeeds\");\n"
            f"    assertTrue(!sys.initialize(\"missing\"), \"missing entity returns false\");\n"
            f"}}\n\n"
            f"void run_{_to_snake(system_name)}_tests() {{\n"
            f"    test{system_name}Init();\n"
            f"}}\n"
        )

    def check_media_files(self, asset_dir):
        """Validate asset files in asset_dir; return list of issues."""
        valid_exts = {".png", ".jpg", ".jpeg", ".dds", ".wav", ".ogg", ".mp3",
                      ".glb", ".fbx", ".obj", ".glsl", ".hlsl", ".json"}
        issues = []
        for f in Path(asset_dir).rglob("*"):
            if f.is_file():
                if f.suffix.lower() not in valid_exts:
                    issues.append(f"Unknown extension: {f}")
                elif f.stat().st_size == 0:
                    issues.append(f"Empty file: {f}")
        return issues

    def validate_json_schema(self, json_path, schema_path=None):
        """Validate a JSON file is well-formed; returns (ok, error_msg)."""
        try:
            with open(json_path, encoding="utf-8") as fh:
                json.load(fh)
            return True, None
        except (json.JSONDecodeError, OSError) as e:
            return False, str(e)

    def run_build_and_capture(self, build_dir, target=None):
        """Run cmake --build and return (success, output)."""
        cmd = ["cmake", "--build", build_dir, "--parallel"]
        if target:
            cmd += ["--target", target]
        try:
            r = subprocess.run(cmd, capture_output=True, text=True, timeout=600)
            return r.returncode == 0, r.stdout + r.stderr
        except Exception as e:
            return False, str(e)

    def analyse_build_output(self, output):
        """Return a dict with error/warning counts from cmake build output."""
        lines = output.splitlines()
        errors = [l for l in lines if "error:" in l.lower()]
        warnings = [l for l in lines if "warning:" in l.lower()]
        return {"errors": len(errors), "warnings": len(warnings),
                "error_lines": errors[:10], "warning_lines": warnings[:10]}


def _to_snake(name):
    """Convert PascalCase to snake_case."""
    import re
    s = re.sub(r"(?<=[a-z0-9])([A-Z])", r"_\1", name)
    return s.lower()

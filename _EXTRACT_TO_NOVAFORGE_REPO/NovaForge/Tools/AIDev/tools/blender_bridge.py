"""
NovaForge Dev AI — Blender Bridge (Phase 6)

Generates 3D props, ships, interiors, and other assets by calling
the Blender Python API headlessly.  Integrates with the existing
BlenderSpaceshipGenerator addon pipeline in tools/.
"""

import json
import logging
import tempfile
from pathlib import Path
from typing import Optional

log = logging.getLogger(__name__)


class BlenderBridge:
    """
    Drives Blender headlessly to generate 3D assets from AI prompts.

    The bridge writes a Blender Python script to a temp file and
    invokes Blender via the CLI.  The generated asset is exported to
    assets/props/generated/ (or the requested output path).

    Phase 6 prerequisites:
        - Blender on PATH (or BLENDER_PATH env var)
        - The NovaForge BlenderSpaceshipGenerator addon loaded
    """

    def __init__(self, repo_root: Path):
        self.repo_root = repo_root
        self.generated_dir = repo_root / "assets" / "props" / "generated"
        self.generated_dir.mkdir(parents=True, exist_ok=True)
        self._blender_exe = self._find_blender()

    # ------------------------------------------------------------------
    # Public
    # ------------------------------------------------------------------

    def is_available(self) -> bool:
        """Return True if Blender is accessible on this system."""
        return self._blender_exe is not None

    def generate_prop(
        self,
        name: str,
        description: str,
        dimensions_m: Optional[tuple] = None,
        style_tags: Optional[list] = None,
        output_path: Optional[str] = None,
    ) -> Optional[str]:
        """
        Generate a single 3D prop via Blender.

        Args:
            name:         Asset name (used for the output filename).
            description:  Natural language description fed to the generator.
            dimensions_m: (width, height, depth) in metres. None = AI decides.
            style_tags:   Style hints (e.g. ["sci-fi", "metallic", "worn"]).
            output_path:  Relative output path for .glb. Default: assets/props/generated/.

        Returns:
            Relative path to the generated .glb, or None on failure.
        """
        if not self.is_available():
            log.error(
                "Blender not found. Install Blender and ensure it is on PATH "
                "or set BLENDER_PATH environment variable."
            )
            return None

        out = (
            self.repo_root / output_path
            if output_path
            else self.generated_dir / f"{name.lower().replace(' ', '_')}.glb"
        )

        script = self._build_prop_script(name, description, dimensions_m, style_tags, str(out))
        return self._run_blender_script(script, out)

    def generate_ship(
        self,
        faction: str = "generic",
        ship_class: str = "frigate",
        output_path: Optional[str] = None,
    ) -> Optional[str]:
        """
        Generate a procedural ship hull using the existing BlenderSpaceshipGenerator.

        Args:
            faction:    Faction name (affects shape language).
            ship_class: "shuttle", "frigate", "cruiser", "battleship", etc.
            output_path: Relative output path for .glb.

        Returns:
            Relative path to the generated .glb, or None on failure.
        """
        if not self.is_available():
            log.error("Blender not found.")
            return None

        generator_script = (
            self.repo_root
            / "tools" / "BlenderSpaceshipGenerator" / "generate_ship_cli.py"
        )
        if not generator_script.exists():
            log.error(f"Ship generator script not found: {generator_script}")
            return None

        out = (
            self.repo_root / output_path
            if output_path
            else self.generated_dir / f"ship_{faction}_{ship_class}.glb"
        )

        params = json.dumps({"faction": faction, "ship_class": ship_class, "output": str(out)})
        script = f"""
import bpy, sys, json
sys.path.insert(0, r"{self.repo_root / 'tools' / 'BlenderSpaceshipGenerator'}")
import generate_ship_cli
params = json.loads(r'{params}')
generate_ship_cli.generate(**params)
"""
        return self._run_blender_script(script, out)

    # ------------------------------------------------------------------
    # Private helpers
    # ------------------------------------------------------------------

    def _build_prop_script(
        self,
        name: str,
        description: str,
        dimensions_m: Optional[tuple],
        style_tags: Optional[list],
        output_path: str,
    ) -> str:
        """Build a minimal Blender Python script for prop generation."""
        w, h, d = dimensions_m if dimensions_m else (1.0, 1.0, 1.0)
        tags = json.dumps(style_tags or [])
        return f"""
import bpy

# Clear default scene
bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete()

# TODO (Phase 6): Replace with LLM-driven Blender geometry generation
# For now, create a labelled placeholder cube at the requested dimensions
bpy.ops.mesh.primitive_cube_add(size=1)
obj = bpy.context.active_object
obj.name = "{name}"
obj.scale = ({w / 2}, {d / 2}, {h / 2})

# Tag with custom properties
obj["ai_description"] = "{description}"
obj["ai_style_tags"] = {tags}

# Export as GLB
bpy.ops.export_scene.gltf(
    filepath=r"{output_path}",
    export_format="GLB",
    use_selection=True,
)
print("Export complete:", r"{output_path}")
"""

    def _run_blender_script(self, script: str, expected_output: Path) -> Optional[str]:
        """Write script to a temp file and invoke Blender CLI."""
        from tools.build_runner import BuildRunner
        runner = BuildRunner(self.repo_root)

        with tempfile.NamedTemporaryFile(
            mode="w", suffix=".py", delete=False, encoding="utf-8"
        ) as f:
            f.write(script)
            temp_path = f.name

        try:
            import os
            cmd = [self._blender_exe, "--background", "--python", temp_path]
            out, ok = runner.run_command(cmd)
            if ok and expected_output.exists():
                rel = str(expected_output.relative_to(self.repo_root))
                log.info(f"[BlenderBridge] Generated: {rel}")
                return rel
            else:
                log.error(f"[BlenderBridge] Generation failed:\n{out}")
                return None
        finally:
            try:
                import os
                os.unlink(temp_path)
            except OSError:
                pass

    @staticmethod
    def _find_blender() -> Optional[str]:
        """Locate the Blender executable."""
        import os
        import shutil
        custom = os.environ.get("BLENDER_PATH")
        if custom and Path(custom).exists():
            return custom
        found = shutil.which("blender")
        if found:
            return found
        # Common Windows paths
        for candidate in [
            r"C:\Program Files\Blender Foundation\Blender 4.2\blender.exe",
            r"C:\Program Files\Blender Foundation\Blender 4.0\blender.exe",
            r"C:\Program Files\Blender Foundation\Blender 3.6\blender.exe",
        ]:
            if Path(candidate).exists():
                return candidate
        return None

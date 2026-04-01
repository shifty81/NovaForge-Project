"""
Zip Tools — compression and packaging utilities.

Provides real zip create/extract/list operations via the stdlib `zipfile`
module, plus helpers for creating versioned release archives.
"""

import os
import zipfile
from pathlib import Path


class ZipTools:
    """Real zip/archive operations using Python's stdlib zipfile."""

    # ------------------------------------------------------------------
    def zip_create(self, output_path, files_or_dirs):
        """Create a zip archive containing the given files/directories."""
        with zipfile.ZipFile(output_path, "w", zipfile.ZIP_DEFLATED) as zf:
            for item in files_or_dirs:
                p = Path(item)
                if p.is_dir():
                    for f in p.rglob("*"):
                        if f.is_file():
                            zf.write(f, f.relative_to(p.parent))
                elif p.is_file():
                    zf.write(p, p.name)
        print(f"[ZipTools] created {output_path}")
        return output_path

    def zip_extract(self, zip_path, output_dir):
        """Extract all contents of a zip archive to output_dir."""
        Path(output_dir).mkdir(parents=True, exist_ok=True)
        with zipfile.ZipFile(zip_path, "r") as zf:
            zf.extractall(output_dir)
        print(f"[ZipTools] extracted {zip_path} → {output_dir}")
        return output_dir

    def zip_add_file(self, zip_path, file_path, arcname=None):
        """Append a single file to an existing (or new) zip archive."""
        mode = "a" if Path(zip_path).exists() else "w"
        with zipfile.ZipFile(zip_path, mode, zipfile.ZIP_DEFLATED) as zf:
            zf.write(file_path, arcname or Path(file_path).name)
        print(f"[ZipTools] added {file_path} → {zip_path}")
        return zip_path

    def zip_pack_folder(self, folder_path, output_path):
        """Pack an entire folder recursively into a zip."""
        folder = Path(folder_path)
        with zipfile.ZipFile(output_path, "w", zipfile.ZIP_DEFLATED) as zf:
            for f in folder.rglob("*"):
                if f.is_file():
                    zf.write(f, f.relative_to(folder.parent))
        print(f"[ZipTools] packed {folder_path} → {output_path}")
        return output_path

    def zip_list(self, zip_path):
        """Return a list of file names inside a zip archive."""
        with zipfile.ZipFile(zip_path, "r") as zf:
            names = zf.namelist()
        return names

    def create_release_zip(self, source_dir, version, output_dir):
        """Pack source_dir into a versioned release zip in output_dir."""
        Path(output_dir).mkdir(parents=True, exist_ok=True)
        name = Path(source_dir).name
        out = str(Path(output_dir) / f"{name}-{version}.zip")
        return self.zip_pack_folder(source_dir, out)

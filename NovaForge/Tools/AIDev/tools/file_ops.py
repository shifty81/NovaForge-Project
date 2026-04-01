"""
NovaForge Dev AI — File Operations (Phase 0)

Safe read / write / snapshot / rollback for project files.
Every file touched by the AI is snapshotted first so you can always undo.
"""

import os
import shutil
import logging
from datetime import datetime
from pathlib import Path
from typing import Optional

log = logging.getLogger(__name__)


class FileOps:
    """
    Safe file I/O for the AI agent.

    All writes go through write_file(), which automatically snapshots the
    original before overwriting.  Snapshots are stored in
    ai_dev/workspace/snapshots/<label>/<timestamp>/ and can be rolled back
    at any time.
    """

    def __init__(self, repo_root: Path):
        self.repo_root = repo_root
        self.snapshots_dir = repo_root / "ai_dev" / "workspace" / "snapshots"
        self.snapshots_dir.mkdir(parents=True, exist_ok=True)

    # ------------------------------------------------------------------
    # Reading
    # ------------------------------------------------------------------

    def read_file(self, rel_path: str) -> Optional[str]:
        """Read a file relative to the repo root. Returns None if missing."""
        full = self.repo_root / rel_path
        if not full.exists():
            log.warning(f"File not found: {rel_path}")
            return None
        try:
            return full.read_text(encoding="utf-8", errors="replace")
        except OSError as e:
            log.error(f"Cannot read {rel_path}: {e}")
            return None

    def list_dir(self, rel_path: str = "") -> list:
        """List files and directories under rel_path (relative to repo root)."""
        target = self.repo_root / rel_path
        if not target.is_dir():
            return []
        entries = []
        for item in sorted(target.iterdir()):
            rel = str(item.relative_to(self.repo_root))
            entries.append({"path": rel, "is_dir": item.is_dir(), "size": item.stat().st_size if item.is_file() else 0})
        return entries

    def file_exists(self, rel_path: str) -> bool:
        return (self.repo_root / rel_path).exists()

    # ------------------------------------------------------------------
    # Writing
    # ------------------------------------------------------------------

    def write_file(self, rel_path: str, content: str, snapshot_label: str = "auto") -> bool:
        """
        Write content to rel_path (relative to repo root).
        If the file already exists, snapshot it first.
        Returns True on success.
        """
        full = self.repo_root / rel_path

        # Snapshot existing file before overwriting
        if full.exists():
            self._snapshot_single_file(rel_path, snapshot_label)

        # Ensure parent directory exists
        full.parent.mkdir(parents=True, exist_ok=True)

        try:
            full.write_text(content, encoding="utf-8")
            log.info(f"Written: {rel_path}")
            return True
        except OSError as e:
            log.error(f"Cannot write {rel_path}: {e}")
            return False

    def create_file(self, rel_path: str, content: str) -> bool:
        """Create a new file. Fails if it already exists (use write_file to overwrite)."""
        full = self.repo_root / rel_path
        if full.exists():
            log.warning(f"File already exists, use write_file to overwrite: {rel_path}")
            return False
        return self.write_file(rel_path, content)

    def delete_file(self, rel_path: str, snapshot_label: str = "before_delete") -> bool:
        """Delete a file (snapshot it first)."""
        full = self.repo_root / rel_path
        if not full.exists():
            log.warning(f"Cannot delete — not found: {rel_path}")
            return False
        self._snapshot_single_file(rel_path, snapshot_label)
        try:
            full.unlink()
            log.info(f"Deleted: {rel_path}")
            return True
        except OSError as e:
            log.error(f"Cannot delete {rel_path}: {e}")
            return False

    def apply_diff(self, rel_path: str, old_str: str, new_str: str) -> bool:
        """
        Apply a surgical string replacement in a file.
        Equivalent to find-and-replace one exact occurrence.
        """
        content = self.read_file(rel_path)
        if content is None:
            return False
        if old_str not in content:
            log.warning(f"Patch target not found in {rel_path}.")
            return False
        new_content = content.replace(old_str, new_str, 1)
        return self.write_file(rel_path, new_content)

    # ------------------------------------------------------------------
    # Snapshots
    # ------------------------------------------------------------------

    def snapshot_workspace(self, label: str = "manual") -> str:
        """
        Snapshot all currently modified (tracked) source files.
        Returns the snapshot directory path.
        """
        ts = datetime.now().strftime("%Y%m%d_%H%M%S")
        snap_dir = self.snapshots_dir / f"{label}_{ts}"
        snap_dir.mkdir(parents=True, exist_ok=True)

        # Track which files are currently under ai_dev workspace management
        manifest_path = self.snapshots_dir / "last_snapshot.txt"
        if manifest_path.exists():
            files = manifest_path.read_text(encoding="utf-8").splitlines()
            for rel in files:
                self._snapshot_single_file(rel, label, ts)

        log.info(f"Workspace snapshotted → {snap_dir}")
        return str(snap_dir)

    def list_snapshots(self) -> list:
        """Return a sorted list of snapshot directory names."""
        if not self.snapshots_dir.exists():
            return []
        snaps = [d.name for d in sorted(self.snapshots_dir.iterdir()) if d.is_dir()]
        return snaps

    def rollback_snapshot(self, snapshot_name: str) -> bool:
        """
        Restore all files from the given snapshot directory.
        Returns True if at least one file was restored.
        """
        snap_dir = self.snapshots_dir / snapshot_name
        if not snap_dir.is_dir():
            log.error(f"Snapshot not found: {snapshot_name}")
            return False

        restored = 0
        for snap_file in snap_dir.rglob("*"):
            if not snap_file.is_file():
                continue
            rel = str(snap_file.relative_to(snap_dir))
            dest = self.repo_root / rel
            dest.parent.mkdir(parents=True, exist_ok=True)
            try:
                shutil.copy2(snap_file, dest)
                restored += 1
            except OSError as e:
                log.warning(f"Could not restore {rel}: {e}")

        log.info(f"Rollback complete: restored {restored} file(s) from '{snapshot_name}'")
        return restored > 0

    # ------------------------------------------------------------------
    # Private helpers
    # ------------------------------------------------------------------

    def _snapshot_single_file(self, rel_path: str, label: str, ts: Optional[str] = None) -> bool:
        """Copy a single file into a snapshot sub-directory."""
        full = self.repo_root / rel_path
        if not full.exists():
            return False
        if ts is None:
            ts = datetime.now().strftime("%Y%m%d_%H%M%S")
        snap_dest = self.snapshots_dir / f"{label}_{ts}" / rel_path
        snap_dest.parent.mkdir(parents=True, exist_ok=True)
        try:
            shutil.copy2(full, snap_dest)
            return True
        except OSError:
            return False

"""
NovaForge Dev AI — Hot Reload Manager (Phase 1 + Phase 2)

Watches source files for changes and triggers appropriate reload mechanisms
without restarting the full engine. Supports:
  - Lua scripts (dofile-based reload via IPC)
  - Python modules (importlib.reload)
  - GLSL shaders (compile + signal engine via IPC)
  - JSON data files (re-parse + notify server via IPC)
  - UI panels (hot-reload via IPC)
  - C++ dynamic libraries (rebuild + signal)

Phase 2 additions:
  - IPC bridge integration for Lua, GLSL, JSON, and UI reloads
  - Graceful fallback when the engine is not running
  - Batch reload support for multiple files
  - Watch statistics tracking
"""

import os
import sys
import time
import logging
import importlib
import importlib.util
import threading
from pathlib import Path
from typing import Callable, Optional

from tools.ipc_bridge import IPCBridge

log = logging.getLogger(__name__)


class HotReloadManager:
    """
    Watches files and triggers reload callbacks when they change.

    Usage:
        mgr = HotReloadManager(repo_root)
        mgr.watch("assets/scripts/equipment_panel.lua", "lua")
        mgr.start()
        # ... later:
        mgr.stop()
    """

    def __init__(self, repo_root: Path, ipc: Optional[IPCBridge] = None):
        self.repo_root = repo_root
        self._watches: dict[str, dict] = {}  # rel_path → {type, mtime, callback}
        self._running = False
        self._thread: Optional[threading.Thread] = None
        self._interval = 0.5  # seconds between polls
        self._ipc = ipc or IPCBridge()
        self._stats: dict[str, int] = {}  # rel_path → reload count

    # ------------------------------------------------------------------
    # Watch registration
    # ------------------------------------------------------------------

    def watch(self, rel_path: str, file_type: str = "auto",
              callback: Optional[Callable[[str], None]] = None):
        """
        Register a file for hot-reload watching.

        Args:
            rel_path:  Path relative to repo root.
            file_type: One of "lua", "python", "glsl", "json", "cpp", "auto".
                       "auto" infers from extension.
            callback:  Optional function called with rel_path when file changes.
        """
        if file_type == "auto":
            ext = Path(rel_path).suffix.lower()
            file_type = {
                ".lua": "lua", ".py": "python",
                ".vert": "glsl", ".frag": "glsl", ".glsl": "glsl",
                ".json": "json",
                ".cpp": "cpp", ".h": "cpp",
            }.get(ext, "unknown")

        full = self.repo_root / rel_path
        mtime = full.stat().st_mtime if full.exists() else 0.0
        self._watches[rel_path] = {
            "type": file_type, "mtime": mtime, "callback": callback
        }
        log.debug(f"Watching [{file_type}]: {rel_path}")

    def unwatch(self, rel_path: str):
        self._watches.pop(rel_path, None)

    # ------------------------------------------------------------------
    # Watch loop
    # ------------------------------------------------------------------

    def start(self):
        """Start background polling thread."""
        if self._running:
            return
        self._running = True
        self._thread = threading.Thread(target=self._poll_loop, daemon=True)
        self._thread.start()
        log.info("HotReloadManager started.")

    def stop(self):
        """Stop the background thread."""
        self._running = False
        if self._thread:
            self._thread.join(timeout=2)
        log.info("HotReloadManager stopped.")

    def check_all(self):
        """Manually check all watched files for changes (synchronous)."""
        for rel_path in list(self._watches.keys()):
            self._check_file(rel_path)

    # ------------------------------------------------------------------
    # Manual reload triggers (call from agent or overlay)
    # ------------------------------------------------------------------

    def reload_lua(self, rel_path: str) -> bool:
        """Signal the engine to reload a Lua script via IPC."""
        log.info(f"[HotReload] Lua: {rel_path}")
        ok, msg = self._ipc.reload_lua(rel_path)
        if not ok:
            log.debug(f"IPC unavailable for Lua reload ({msg}); change noted.")
        self._record_reload(rel_path)
        return ok

    def reload_python(self, module_name: str) -> bool:
        """Reload a loaded Python module by dotted name."""
        try:
            mod = sys.modules.get(module_name)
            if mod:
                importlib.reload(mod)
                log.info(f"[HotReload] Python: {module_name}")
                return True
        except Exception as e:
            log.warning(f"Python reload failed for {module_name}: {e}")
        return False

    def reload_glsl(self, rel_path: str) -> bool:
        """Signal the engine to recompile and relink a GLSL shader via IPC."""
        log.info(f"[HotReload] GLSL: {rel_path}")
        ok, msg = self._ipc.reload_glsl(rel_path)
        if not ok:
            log.debug(f"IPC unavailable for GLSL reload ({msg}); change noted.")
        self._record_reload(rel_path)
        return ok

    def reload_json(self, rel_path: str) -> bool:
        """Signal the game server to re-parse a JSON data file via IPC."""
        log.info(f"[HotReload] JSON: {rel_path}")
        ok, msg = self._ipc.reload_json(rel_path)
        if not ok:
            log.debug(f"IPC unavailable for JSON reload ({msg}); change noted.")
        self._record_reload(rel_path)
        return ok

    def reload_ui(self, rel_path: str) -> bool:
        """Signal the engine to hot-reload a UI panel definition via IPC."""
        log.info(f"[HotReload] UI: {rel_path}")
        ok, msg = self._ipc.reload_ui(rel_path)
        if not ok:
            log.debug(f"IPC unavailable for UI reload ({msg}); change noted.")
        self._record_reload(rel_path)
        return ok

    def reload_batch(self, paths: list[str]) -> dict[str, bool]:
        """
        Reload multiple files at once.

        Returns a dict mapping each path to its reload success status.
        """
        results = {}
        for rel_path in paths:
            info = self._watches.get(rel_path)
            ftype = info["type"] if info else self._detect_type(rel_path)
            results[rel_path] = self._reload_by_type(ftype, rel_path)
        return results

    def get_stats(self) -> dict[str, int]:
        """Return reload counts per watched file."""
        return dict(self._stats)

    # ------------------------------------------------------------------
    # Private
    # ------------------------------------------------------------------

    def _poll_loop(self):
        while self._running:
            for rel_path in list(self._watches.keys()):
                self._check_file(rel_path)
            time.sleep(self._interval)

    def _check_file(self, rel_path: str):
        info = self._watches.get(rel_path)
        if not info:
            return
        full = self.repo_root / rel_path
        try:
            mtime = full.stat().st_mtime
        except OSError:
            return
        if mtime == info["mtime"]:
            return
        info["mtime"] = mtime
        log.info(f"[Change detected] {rel_path} — triggering {info['type']} reload")
        self._dispatch(rel_path, info)

    def _dispatch(self, rel_path: str, info: dict):
        ftype = info["type"]
        self._reload_by_type(ftype, rel_path)

        callback = info.get("callback")
        if callback:
            try:
                callback(rel_path)
            except Exception as e:
                log.warning(f"Hot-reload callback error for {rel_path}: {e}")

    def _reload_by_type(self, ftype: str, rel_path: str) -> bool:
        """Dispatch reload to the correct handler by file type."""
        if ftype == "lua":
            return self.reload_lua(rel_path)
        elif ftype == "python":
            mod = Path(rel_path).stem
            return self.reload_python(mod)
        elif ftype == "glsl":
            return self.reload_glsl(rel_path)
        elif ftype == "json":
            return self.reload_json(rel_path)
        elif ftype == "ui":
            return self.reload_ui(rel_path)
        elif ftype == "cpp":
            log.info(f"[HotReload] C++ change — run 'build' to recompile: {rel_path}")
            return False
        return False

    def _detect_type(self, rel_path: str) -> str:
        """Infer file type from extension."""
        ext = Path(rel_path).suffix.lower()
        return {
            ".lua": "lua", ".py": "python",
            ".vert": "glsl", ".frag": "glsl", ".glsl": "glsl",
            ".json": "json", ".xml": "ui",
            ".cpp": "cpp", ".h": "cpp",
        }.get(ext, "unknown")

    def _record_reload(self, rel_path: str):
        """Track how many times each file has been reloaded."""
        self._stats[rel_path] = self._stats.get(rel_path, 0) + 1

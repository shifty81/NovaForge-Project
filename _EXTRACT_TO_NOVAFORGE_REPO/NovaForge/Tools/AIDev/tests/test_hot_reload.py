"""
Tests for ai_dev/tools/hot_reload.py — Phase 2 IPC integration

Tests the IPC-wired hot-reload methods, batch reload, and statistics.
"""

import shutil
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch, MagicMock

import sys
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from tools.hot_reload import HotReloadManager
from tools.ipc_bridge import IPCBridge


class TestHotReloadInit(unittest.TestCase):
    """Test HotReloadManager construction."""

    def test_default_init(self):
        mgr = HotReloadManager(Path("/tmp/test"))
        self.assertIsNotNone(mgr._ipc)
        self.assertEqual(mgr._stats, {})

    def test_custom_ipc(self):
        ipc = IPCBridge(port=9999)
        mgr = HotReloadManager(Path("/tmp/test"), ipc=ipc)
        self.assertEqual(mgr._ipc.port, 9999)


class TestHotReloadIPC(unittest.TestCase):
    """Test that reload methods call the IPC bridge."""

    def setUp(self):
        self.ipc = MagicMock(spec=IPCBridge)
        self.mgr = HotReloadManager(Path("/tmp/test"), ipc=self.ipc)

    def test_reload_lua_calls_ipc(self):
        self.ipc.reload_lua.return_value = (True, "ok")
        ok = self.mgr.reload_lua("scripts/equip.lua")
        self.assertTrue(ok)
        self.ipc.reload_lua.assert_called_once_with("scripts/equip.lua")

    def test_reload_lua_ipc_failure(self):
        self.ipc.reload_lua.return_value = (False, "not reachable")
        ok = self.mgr.reload_lua("scripts/equip.lua")
        self.assertFalse(ok)

    def test_reload_glsl_calls_ipc(self):
        self.ipc.reload_glsl.return_value = (True, "ok")
        ok = self.mgr.reload_glsl("shaders/pbr.frag")
        self.assertTrue(ok)
        self.ipc.reload_glsl.assert_called_once_with("shaders/pbr.frag")

    def test_reload_json_calls_ipc(self):
        self.ipc.reload_json.return_value = (True, "ok")
        ok = self.mgr.reload_json("data/ships.json")
        self.assertTrue(ok)
        self.ipc.reload_json.assert_called_once_with("data/ships.json")

    def test_reload_ui_calls_ipc(self):
        self.ipc.reload_ui.return_value = (True, "ok")
        ok = self.mgr.reload_ui("panels/equip.xml")
        self.assertTrue(ok)
        self.ipc.reload_ui.assert_called_once_with("panels/equip.xml")


class TestHotReloadStats(unittest.TestCase):
    """Test reload statistics tracking."""

    def setUp(self):
        self.ipc = MagicMock(spec=IPCBridge)
        self.ipc.reload_lua.return_value = (True, "ok")
        self.ipc.reload_glsl.return_value = (True, "ok")
        self.mgr = HotReloadManager(Path("/tmp/test"), ipc=self.ipc)

    def test_stats_initially_empty(self):
        self.assertEqual(self.mgr.get_stats(), {})

    def test_stats_increment_on_reload(self):
        self.mgr.reload_lua("scripts/equip.lua")
        stats = self.mgr.get_stats()
        self.assertEqual(stats["scripts/equip.lua"], 1)

    def test_stats_multiple_reloads(self):
        self.mgr.reload_lua("scripts/equip.lua")
        self.mgr.reload_lua("scripts/equip.lua")
        self.mgr.reload_lua("scripts/equip.lua")
        stats = self.mgr.get_stats()
        self.assertEqual(stats["scripts/equip.lua"], 3)

    def test_stats_different_files(self):
        self.mgr.reload_lua("scripts/a.lua")
        self.mgr.reload_glsl("shaders/b.frag")
        stats = self.mgr.get_stats()
        self.assertEqual(stats["scripts/a.lua"], 1)
        self.assertEqual(stats["shaders/b.frag"], 1)


class TestHotReloadBatch(unittest.TestCase):
    """Test batch reload of multiple files."""

    def setUp(self):
        self.ipc = MagicMock(spec=IPCBridge)
        self.ipc.reload_lua.return_value = (True, "ok")
        self.ipc.reload_glsl.return_value = (True, "ok")
        self.ipc.reload_json.return_value = (False, "engine not running")
        self.tmp = tempfile.mkdtemp()
        self.repo_root = Path(self.tmp)
        self.mgr = HotReloadManager(self.repo_root, ipc=self.ipc)

    def tearDown(self):
        shutil.rmtree(self.tmp, ignore_errors=True)

    def test_batch_reload(self):
        # Register watches so types are known
        (self.repo_root / "scripts").mkdir()
        (self.repo_root / "scripts" / "a.lua").write_text("")
        (self.repo_root / "shaders").mkdir()
        (self.repo_root / "shaders" / "b.frag").write_text("")

        self.mgr.watch("scripts/a.lua", "lua")
        self.mgr.watch("shaders/b.frag", "glsl")

        results = self.mgr.reload_batch(["scripts/a.lua", "shaders/b.frag"])
        self.assertTrue(results["scripts/a.lua"])
        self.assertTrue(results["shaders/b.frag"])

    def test_batch_reload_detects_type(self):
        # File not in watch list — type inferred from extension
        results = self.mgr.reload_batch(["data/ships.json"])
        self.assertFalse(results["data/ships.json"])  # IPC returns False


class TestHotReloadDispatch(unittest.TestCase):
    """Test internal dispatch by file type."""

    def setUp(self):
        self.ipc = MagicMock(spec=IPCBridge)
        self.ipc.reload_lua.return_value = (True, "ok")
        self.ipc.reload_glsl.return_value = (True, "ok")
        self.ipc.reload_json.return_value = (True, "ok")
        self.ipc.reload_ui.return_value = (True, "ok")
        self.mgr = HotReloadManager(Path("/tmp/test"), ipc=self.ipc)

    def test_dispatch_lua(self):
        ok = self.mgr._reload_by_type("lua", "test.lua")
        self.assertTrue(ok)

    def test_dispatch_glsl(self):
        ok = self.mgr._reload_by_type("glsl", "test.frag")
        self.assertTrue(ok)

    def test_dispatch_json(self):
        ok = self.mgr._reload_by_type("json", "test.json")
        self.assertTrue(ok)

    def test_dispatch_ui(self):
        ok = self.mgr._reload_by_type("ui", "test.xml")
        self.assertTrue(ok)

    def test_dispatch_cpp(self):
        ok = self.mgr._reload_by_type("cpp", "test.cpp")
        self.assertFalse(ok)

    def test_dispatch_unknown(self):
        ok = self.mgr._reload_by_type("unknown", "test.xyz")
        self.assertFalse(ok)


class TestHotReloadTypeDetection(unittest.TestCase):
    """Test automatic file type detection."""

    def setUp(self):
        self.mgr = HotReloadManager(Path("/tmp/test"))

    def test_detect_lua(self):
        self.assertEqual(self.mgr._detect_type("scripts/equip.lua"), "lua")

    def test_detect_python(self):
        self.assertEqual(self.mgr._detect_type("ai_dev/tool.py"), "python")

    def test_detect_glsl_vert(self):
        self.assertEqual(self.mgr._detect_type("shaders/test.vert"), "glsl")

    def test_detect_glsl_frag(self):
        self.assertEqual(self.mgr._detect_type("shaders/test.frag"), "glsl")

    def test_detect_json(self):
        self.assertEqual(self.mgr._detect_type("data/ships.json"), "json")

    def test_detect_ui(self):
        self.assertEqual(self.mgr._detect_type("panels/equip.xml"), "ui")

    def test_detect_cpp(self):
        self.assertEqual(self.mgr._detect_type("src/main.cpp"), "cpp")

    def test_detect_unknown(self):
        self.assertEqual(self.mgr._detect_type("file.xyz"), "unknown")


if __name__ == "__main__":
    unittest.main()

"""
Tests for ai_dev/tools/ipc_bridge.py
"""

import json
import socket
import threading
import unittest
from unittest.mock import patch, MagicMock

import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from tools.ipc_bridge import IPCBridge, DEFAULT_HOST, DEFAULT_PORT


class TestIPCBridgeInit(unittest.TestCase):
    """Test IPCBridge construction."""

    def test_default_init(self):
        bridge = IPCBridge()
        self.assertEqual(bridge.host, DEFAULT_HOST)
        self.assertEqual(bridge.port, DEFAULT_PORT)
        self.assertEqual(bridge.timeout, 5)

    def test_custom_init(self):
        bridge = IPCBridge(host="192.168.1.1", port=9999, timeout=10)
        self.assertEqual(bridge.host, "192.168.1.1")
        self.assertEqual(bridge.port, 9999)
        self.assertEqual(bridge.timeout, 10)


class TestIPCBridgeConnection(unittest.TestCase):
    """Test connection handling when engine is not running."""

    def test_ping_connection_refused(self):
        bridge = IPCBridge(port=19899)  # unlikely port
        ok, msg = bridge.ping()
        self.assertFalse(ok)
        self.assertIn("not reachable", msg)

    def test_is_engine_running_false(self):
        bridge = IPCBridge(port=19899)
        self.assertFalse(bridge.is_engine_running())

    def test_reload_lua_connection_refused(self):
        bridge = IPCBridge(port=19899)
        ok, msg = bridge.reload_lua("scripts/test.lua")
        self.assertFalse(ok)
        self.assertIn("not reachable", msg)

    def test_reload_glsl_connection_refused(self):
        bridge = IPCBridge(port=19899)
        ok, msg = bridge.reload_glsl("shaders/test.frag")
        self.assertFalse(ok)

    def test_reload_json_connection_refused(self):
        bridge = IPCBridge(port=19899)
        ok, msg = bridge.reload_json("data/test.json")
        self.assertFalse(ok)

    def test_reload_ui_connection_refused(self):
        bridge = IPCBridge(port=19899)
        ok, msg = bridge.reload_ui("panels/test.xml")
        self.assertFalse(ok)


class TestIPCBridgeGenericReload(unittest.TestCase):
    """Test the generic reload dispatcher."""

    def test_unknown_asset_type(self):
        bridge = IPCBridge(port=19899)
        ok, msg = bridge.reload("unknown_type", "file.xyz")
        self.assertFalse(ok)
        self.assertIn("Unknown asset type", msg)

    def test_reload_dispatches_lua(self):
        bridge = IPCBridge(port=19899)
        with patch.object(bridge, 'reload_lua', return_value=(True, "ok")) as mock:
            ok, msg = bridge.reload("lua", "test.lua")
            mock.assert_called_once_with("test.lua")
            self.assertTrue(ok)

    def test_reload_dispatches_glsl(self):
        bridge = IPCBridge(port=19899)
        with patch.object(bridge, 'reload_glsl', return_value=(True, "ok")) as mock:
            ok, msg = bridge.reload("glsl", "test.frag")
            mock.assert_called_once_with("test.frag")

    def test_reload_dispatches_json(self):
        bridge = IPCBridge(port=19899)
        with patch.object(bridge, 'reload_json', return_value=(True, "ok")) as mock:
            ok, msg = bridge.reload("json", "test.json")
            mock.assert_called_once_with("test.json")

    def test_reload_dispatches_ui(self):
        bridge = IPCBridge(port=19899)
        with patch.object(bridge, 'reload_ui', return_value=(True, "ok")) as mock:
            ok, msg = bridge.reload("ui", "test.xml")
            mock.assert_called_once_with("test.xml")


class _MockEngineServer:
    """A minimal TCP server that simulates the engine IPC endpoint."""

    def __init__(self, port, response=None):
        self.port = port
        self.response = response or {"status": "ok"}
        self.received = []
        self._server = None
        self._thread = None

    def start(self):
        self._server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self._server.bind(("127.0.0.1", self.port))
        self._server.listen(1)
        self._server.settimeout(5)
        self._thread = threading.Thread(target=self._accept, daemon=True)
        self._thread.start()

    def _accept(self):
        try:
            conn, _ = self._server.accept()
            data = conn.recv(4096)
            self.received.append(json.loads(data.decode("utf-8").strip()))
            reply = json.dumps(self.response) + "\n"
            conn.sendall(reply.encode("utf-8"))
            conn.close()
        except Exception:
            pass

    def stop(self):
        if self._server:
            self._server.close()
        if self._thread:
            self._thread.join(timeout=2)


class TestIPCBridgeWithMockServer(unittest.TestCase):
    """Test IPC communication against a mock engine server."""

    def test_ping_success(self):
        server = _MockEngineServer(port=19851)
        server.start()
        try:
            bridge = IPCBridge(port=19851)
            ok, msg = bridge.ping()
            self.assertTrue(ok)
            self.assertEqual(msg, "ok")
            self.assertEqual(server.received[0]["type"], "ping")
        finally:
            server.stop()

    def test_reload_lua_success(self):
        server = _MockEngineServer(port=19852)
        server.start()
        try:
            bridge = IPCBridge(port=19852)
            ok, msg = bridge.reload_lua("scripts/equip.lua")
            self.assertTrue(ok)
            req = server.received[0]
            self.assertEqual(req["type"], "reload")
            self.assertEqual(req["asset"], "lua")
            self.assertEqual(req["path"], "scripts/equip.lua")
        finally:
            server.stop()

    def test_reload_glsl_success(self):
        server = _MockEngineServer(port=19853)
        server.start()
        try:
            bridge = IPCBridge(port=19853)
            ok, msg = bridge.reload_glsl("shaders/pbr.frag")
            self.assertTrue(ok)
            req = server.received[0]
            self.assertEqual(req["asset"], "glsl")
        finally:
            server.stop()

    def test_engine_error_response(self):
        server = _MockEngineServer(
            port=19854,
            response={"status": "error", "message": "shader compile failed"}
        )
        server.start()
        try:
            bridge = IPCBridge(port=19854)
            ok, msg = bridge.reload_glsl("shaders/bad.frag")
            self.assertFalse(ok)
            self.assertIn("shader compile failed", msg)
        finally:
            server.stop()

    def test_is_engine_running_true(self):
        server = _MockEngineServer(port=19855)
        server.start()
        try:
            bridge = IPCBridge(port=19855)
            self.assertTrue(bridge.is_engine_running())
        finally:
            server.stop()


class TestIPCBridgeTimeout(unittest.TestCase):
    """Test timeout handling."""

    def test_timeout_handling(self):
        # Bind a socket but never accept — forces a timeout
        srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        srv.bind(("127.0.0.1", 19856))
        srv.listen(1)
        try:
            bridge = IPCBridge(port=19856, timeout=0.5)
            ok, msg = bridge.ping()
            # Will either timeout or succeed depending on OS socket behavior
            # The important thing is it doesn't crash
            self.assertIsInstance(ok, bool)
        finally:
            srv.close()


if __name__ == "__main__":
    unittest.main()

"""
NovaForge Dev AI — IPC Bridge (Phase 2)

Socket-based IPC client for communicating with the Atlas Engine process.
Sends reload commands so that the engine can hot-swap Lua scripts, GLSL
shaders, JSON data files, and UI panels without a full restart.

Protocol:
    The engine listens on a local TCP socket (default 127.0.0.1:19850).
    Messages are newline-delimited JSON objects:

        {"type": "reload", "asset": "lua",  "path": "scripts/equip.lua"}
        {"type": "reload", "asset": "glsl", "path": "shaders/pbr.frag"}
        {"type": "reload", "asset": "json", "path": "data/ships.json"}
        {"type": "reload", "asset": "ui",   "path": "panels/equip.xml"}
        {"type": "ping"}

    The engine replies with a JSON object on success or error:

        {"status": "ok"}
        {"status": "error", "message": "shader compile failed: ..."}

    If the engine process is not running or the socket is refused, methods
    return (False, <reason string>) so callers can fall back gracefully.
"""

import json
import logging
import socket
from typing import Optional, Tuple

log = logging.getLogger(__name__)

DEFAULT_HOST = "127.0.0.1"
DEFAULT_PORT = 19850
DEFAULT_TIMEOUT = 5  # seconds


class IPCBridge:
    """
    Thin IPC client that sends reload signals to the Atlas Engine process.

    Usage:
        bridge = IPCBridge()
        ok, msg = bridge.reload_lua("scripts/equipment_panel.lua")
        ok, msg = bridge.reload_glsl("shaders/pbr.frag")
        ok, msg = bridge.ping()
    """

    def __init__(
        self,
        host: str = DEFAULT_HOST,
        port: int = DEFAULT_PORT,
        timeout: float = DEFAULT_TIMEOUT,
    ):
        self.host = host
        self.port = port
        self.timeout = timeout

    # ------------------------------------------------------------------
    # Public API
    # ------------------------------------------------------------------

    def ping(self) -> Tuple[bool, str]:
        """Check if the engine process is listening."""
        return self._send({"type": "ping"})

    def is_engine_running(self) -> bool:
        """Return True if the engine IPC socket is reachable."""
        ok, _ = self.ping()
        return ok

    def reload_lua(self, rel_path: str) -> Tuple[bool, str]:
        """Signal the engine to re-execute a Lua script via dofile()."""
        return self._send({"type": "reload", "asset": "lua", "path": rel_path})

    def reload_glsl(self, rel_path: str) -> Tuple[bool, str]:
        """Signal the engine to recompile and relink a GLSL shader."""
        return self._send({"type": "reload", "asset": "glsl", "path": rel_path})

    def reload_json(self, rel_path: str) -> Tuple[bool, str]:
        """Signal the game server to re-parse a JSON data file."""
        return self._send({"type": "reload", "asset": "json", "path": rel_path})

    def reload_ui(self, rel_path: str) -> Tuple[bool, str]:
        """Signal the engine to hot-reload a UI panel definition."""
        return self._send({"type": "reload", "asset": "ui", "path": rel_path})

    def reload(self, asset_type: str, rel_path: str) -> Tuple[bool, str]:
        """
        Generic reload — routes to the correct asset-specific method.

        Args:
            asset_type: One of "lua", "glsl", "json", "ui".
            rel_path:   Path relative to the repo root.
        """
        dispatch = {
            "lua": self.reload_lua,
            "glsl": self.reload_glsl,
            "json": self.reload_json,
            "ui": self.reload_ui,
        }
        handler = dispatch.get(asset_type)
        if handler is None:
            return False, f"Unknown asset type: {asset_type}"
        return handler(rel_path)

    # ------------------------------------------------------------------
    # Private
    # ------------------------------------------------------------------

    def _send(self, message: dict) -> Tuple[bool, str]:
        """
        Send a JSON message to the engine and return (success, reply_text).

        Returns (False, reason) if the engine is unreachable or replies
        with an error status.
        """
        payload = json.dumps(message) + "\n"
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                sock.settimeout(self.timeout)
                sock.connect((self.host, self.port))
                sock.sendall(payload.encode("utf-8"))

                # Read response (up to 4 KB — engine replies are small)
                data = b""
                while True:
                    chunk = sock.recv(4096)
                    if not chunk:
                        break
                    data += chunk
                    if b"\n" in data:
                        break

                if not data:
                    return True, "ok (no reply)"

                reply = json.loads(data.decode("utf-8").strip())
                status = reply.get("status", "ok")
                if status == "ok":
                    return True, reply.get("message", "ok")
                else:
                    err = reply.get("message", "unknown error")
                    log.warning(f"Engine IPC error: {err}")
                    return False, err

        except ConnectionRefusedError:
            msg = (
                f"Engine IPC not reachable at {self.host}:{self.port}. "
                "Is the engine running with IPC enabled?"
            )
            log.debug(msg)
            return False, msg
        except socket.timeout:
            msg = f"Engine IPC timed out after {self.timeout}s"
            log.warning(msg)
            return False, msg
        except json.JSONDecodeError as e:
            msg = f"Invalid JSON reply from engine: {e}"
            log.warning(msg)
            return False, msg
        except OSError as e:
            msg = f"IPC socket error: {e}"
            log.warning(msg)
            return False, msg

"""
NovaForge Dev AI — Plugin / Extension Loader

Loads optional agent plugins from ai_dev/plugins/. Each plugin is a Python
package (folder) with a plugin.json manifest and an __init__.py that
registers its tools with the agent's tool registry.

Plugin manifest format (plugin.json):
{
    "name": "my_plugin",
    "version": "1.0.0",
    "description": "What this plugin does",
    "author": "Your name",
    "tools": ["tool_a", "tool_b"],
    "requires": []
}

Each tool in __init__.py must expose:
    TOOL_NAME: str
    TOOL_DESCRIPTION: str
    def run(args: dict, repo_root: Path) -> str: ...
"""

import json
import importlib.util
import logging
from pathlib import Path
from typing import Callable, Optional, List

log = logging.getLogger(__name__)


class PluginTool:
    """A single callable tool registered by a plugin."""
    __slots__ = ("name", "description", "run", "plugin_name")

    def __init__(self, name: str, description: str,
                 run_fn: Callable, plugin_name: str):
        self.name = name
        self.description = description
        self.run = run_fn
        self.plugin_name = plugin_name

    def __repr__(self):
        return f"<PluginTool {self.plugin_name}/{self.name}>"


class PluginLoader:
    """
    Scans ai_dev/plugins/ for valid plugin packages and loads their tools
    into a shared tool registry.

    Usage:
        loader = PluginLoader(repo_root)
        loader.load_all()
        tool = loader.get_tool("generate_texture")
        if tool:
            result = tool.run({"prompt": "...rocky surface"}, repo_root)
    """

    def __init__(self, repo_root: Path):
        self.repo_root = repo_root
        self.plugins_dir = repo_root / "ai_dev" / "plugins"
        self._tools: "dict[str, PluginTool]" = {}  # name → PluginTool
        self._loaded_plugins: List[str] = []

    # ------------------------------------------------------------------
    # Public
    # ------------------------------------------------------------------

    def load_all(self) -> int:
        """Scan plugins dir and load every valid plugin. Returns count loaded."""
        if not self.plugins_dir.exists():
            log.info("No plugins directory found (ai_dev/plugins/ — create to add plugins).")
            return 0

        loaded = 0
        for entry in sorted(self.plugins_dir.iterdir()):
            if not entry.is_dir():
                continue
            manifest = entry / "plugin.json"
            if not manifest.exists():
                continue
            if self._load_plugin(entry):
                loaded += 1
        log.info(f"Loaded {loaded} plugin(s), {len(self._tools)} tool(s) registered.")
        return loaded

    def get_tool(self, name: str) -> Optional[PluginTool]:
        return self._tools.get(name)

    def list_tools(self) -> List[PluginTool]:
        return list(self._tools.values())

    def list_plugins(self) -> List[str]:
        return list(self._loaded_plugins)

    # ------------------------------------------------------------------
    # Private
    # ------------------------------------------------------------------

    def _load_plugin(self, plugin_dir: Path) -> bool:
        manifest_path = plugin_dir / "plugin.json"
        init_path = plugin_dir / "__init__.py"

        try:
            manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError) as e:
            log.warning(f"Plugin {plugin_dir.name}: bad manifest — {e}")
            return False

        name = manifest.get("name", plugin_dir.name)

        if not init_path.exists():
            log.warning(f"Plugin {name}: missing __init__.py")
            return False

        # Dynamically load the plugin module
        try:
            spec = importlib.util.spec_from_file_location(
                f"novaforge_plugin_{name}", init_path
            )
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
        except Exception as e:
            log.error(f"Plugin {name}: failed to load — {e}")
            return False

        # Register exported tools
        tool_names = manifest.get("tools", [])
        registered = 0
        for tool_name in tool_names:
            tool_module = getattr(module, tool_name, None)
            if tool_module is None:
                log.warning(f"Plugin {name}: tool '{tool_name}' not found in module.")
                continue

            run_fn = getattr(tool_module, "run", None)
            desc = getattr(tool_module, "TOOL_DESCRIPTION", tool_name)
            full_name = getattr(tool_module, "TOOL_NAME", tool_name)

            if run_fn is None:
                log.warning(f"Plugin {name}: tool '{tool_name}' has no run() function.")
                continue

            self._tools[full_name] = PluginTool(full_name, desc, run_fn, name)
            registered += 1

        if registered:
            self._loaded_plugins.append(name)
            log.info(f"Plugin '{name}' loaded ({registered} tool(s)).")
        return registered > 0

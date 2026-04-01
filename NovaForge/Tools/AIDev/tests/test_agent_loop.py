"""
Tests for ai_dev/core/agent_loop.py

Tests the non-interactive parts of the agent loop: file change extraction,
auto-approve mode, CLI argument parsing, and command dispatch.
"""

import json
import os
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch, MagicMock

import sys
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from core.agent_loop import AgentLoop


class TestExtractFileChanges(unittest.TestCase):
    """Test the code-block extraction logic used by the agent loop."""

    def test_single_cpp_block(self):
        response = (
            "Here's the fix:\n"
            "```cpp cpp_server/src/systems/foo_system.cpp\n"
            "int x = 1;\n"
            "```\n"
        )
        changes = AgentLoop._extract_file_changes(response)
        self.assertEqual(len(changes), 1)
        self.assertEqual(changes[0]["path"], "cpp_server/src/systems/foo_system.cpp")
        self.assertEqual(changes[0]["content"], "int x = 1;")

    def test_multiple_blocks(self):
        response = (
            "```cpp cpp_server/include/systems/a.h\n"
            "#pragma once\n"
            "class A {};\n"
            "```\n"
            "\n"
            "```cpp cpp_server/src/systems/a.cpp\n"
            "#include \"systems/a.h\"\n"
            "```\n"
        )
        changes = AgentLoop._extract_file_changes(response)
        self.assertEqual(len(changes), 2)
        self.assertEqual(changes[0]["path"], "cpp_server/include/systems/a.h")
        self.assertEqual(changes[1]["path"], "cpp_server/src/systems/a.cpp")

    def test_no_path_in_block(self):
        response = "```cpp\nint x = 1;\n```\n"
        changes = AgentLoop._extract_file_changes(response)
        self.assertEqual(len(changes), 0)

    def test_path_only_format(self):
        response = "```cpp_server/src/main.cpp\nint main() {}\n```\n"
        changes = AgentLoop._extract_file_changes(response)
        self.assertEqual(len(changes), 1)
        self.assertEqual(changes[0]["path"], "cpp_server/src/main.cpp")

    def test_empty_response(self):
        changes = AgentLoop._extract_file_changes("")
        self.assertEqual(len(changes), 0)

    def test_python_block(self):
        response = "```python ai_dev/tools/new_tool.py\nprint('hello')\n```\n"
        changes = AgentLoop._extract_file_changes(response)
        self.assertEqual(len(changes), 1)
        self.assertEqual(changes[0]["path"], "ai_dev/tools/new_tool.py")

    def test_unclosed_block(self):
        response = "```cpp cpp_server/src/x.cpp\nint x = 1;\n"
        changes = AgentLoop._extract_file_changes(response)
        # Unclosed block should not produce a change
        self.assertEqual(len(changes), 0)

    def test_mixed_closed_and_unclosed_blocks(self):
        response = (
            "```cpp cpp_server/src/a.cpp\nint a = 1;\n```\n"
            "```cpp cpp_server/src/b.cpp\nint b = 2;\n"
        )
        changes = AgentLoop._extract_file_changes(response)
        # Only the closed block should be extracted
        self.assertEqual(len(changes), 1)
        self.assertEqual(changes[0]["path"], "cpp_server/src/a.cpp")


class TestAgentLoopInit(unittest.TestCase):
    """Test AgentLoop construction and configuration."""

    def test_default_init(self):
        agent = AgentLoop()
        self.assertFalse(agent.auto_approve)
        self.assertEqual(agent.max_iterations, 5)

    def test_auto_approve_init(self):
        agent = AgentLoop(auto_approve=True, max_iterations=10)
        self.assertTrue(agent.auto_approve)
        self.assertEqual(agent.max_iterations, 10)


class TestSessionPersistence(unittest.TestCase):
    """Test session log save/load."""

    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.session_file = Path(self.tmp) / "session_log.json"

    def tearDown(self):
        import shutil
        shutil.rmtree(self.tmp, ignore_errors=True)

    def test_save_and_load(self):
        agent = AgentLoop()
        agent.session_history = [
            {"role": "user", "content": "test prompt"},
            {"role": "assistant", "content": "test response"},
        ]

        # Patch SESSION_LOG to our temp path
        import core.agent_loop as al
        original = al.SESSION_LOG
        try:
            al.SESSION_LOG = self.session_file
            al.WORKSPACE_DIR = Path(self.tmp)
            agent._save_session_log()

            # Verify file was written
            self.assertTrue(self.session_file.exists())
            data = json.loads(self.session_file.read_text())
            self.assertEqual(len(data), 2)
            self.assertEqual(data[0]["content"], "test prompt")

            # Load into new agent
            agent2 = AgentLoop()
            agent2._load_session_log()
            self.assertEqual(len(agent2.session_history), 2)
        finally:
            al.SESSION_LOG = original


class TestAutoApprove(unittest.TestCase):
    """Test that auto-approve skips user confirmation."""

    def test_ask_yes_no_auto(self):
        agent = AgentLoop(auto_approve=True)
        self.assertTrue(agent._ask_yes_no("any question"))


class TestDispatch(unittest.TestCase):
    """Test command routing in _dispatch."""

    def test_help_dispatch(self):
        agent = AgentLoop()
        with patch.object(agent, '_print_help') as mock_help:
            agent._dispatch("help")
            mock_help.assert_called_once()

    def test_build_dispatch(self):
        agent = AgentLoop()
        with patch.object(agent, '_cmd_build') as mock_build:
            agent._dispatch("build")
            mock_build.assert_called_once()

    def test_status_dispatch(self):
        agent = AgentLoop()
        with patch.object(agent, '_cmd_status') as mock_status:
            agent._dispatch("status")
            mock_status.assert_called_once()

    def test_missing_dispatch(self):
        agent = AgentLoop()
        with patch.object(agent, '_cmd_missing') as mock_missing:
            agent._dispatch("missing")
            mock_missing.assert_called_once()

    def test_test_dispatch(self):
        agent = AgentLoop()
        with patch.object(agent, '_cmd_test') as mock_test:
            agent._dispatch("test my_test")
            mock_test.assert_called_once_with("my_test")

    def test_test_dispatch_no_name(self):
        agent = AgentLoop()
        with patch.object(agent, '_cmd_test') as mock_test:
            agent._dispatch("test")
            mock_test.assert_called_once_with(None)

    def test_auto_dispatch(self):
        agent = AgentLoop()
        with patch.object(agent, '_cmd_auto_iterate') as mock_auto:
            agent._dispatch("auto fix the build errors")
            mock_auto.assert_called_once_with("fix the build errors")

    def test_scaffold_dispatch(self):
        agent = AgentLoop()
        with patch.object(agent, '_cmd_scaffold') as mock_scaffold:
            agent._dispatch("scaffold fleet_debrief fleet_components.h")
            mock_scaffold.assert_called_once_with("fleet_debrief fleet_components.h")

    def test_git_dispatch(self):
        agent = AgentLoop()
        with patch.object(agent, '_cmd_git') as mock_git:
            agent._dispatch("git status")
            mock_git.assert_called_once_with("status")

    def test_clear_dispatch(self):
        agent = AgentLoop()
        agent.session_history = [{"role": "user", "content": "test"}]
        agent._dispatch("clear")
        self.assertEqual(len(agent.session_history), 0)

    def test_prompt_dispatch(self):
        agent = AgentLoop()
        with patch.object(agent, '_handle_prompt') as mock_prompt:
            agent._dispatch("Show me all ECS systems")
            mock_prompt.assert_called_once_with("Show me all ECS systems")

    def test_watch_dispatch(self):
        agent = AgentLoop()
        with patch.object(agent, '_cmd_watch') as mock_watch:
            agent._dispatch("watch scripts/equip.lua")
            mock_watch.assert_called_once_with("scripts/equip.lua")

    def test_reload_dispatch(self):
        agent = AgentLoop()
        with patch.object(agent, '_cmd_reload') as mock_reload:
            agent._dispatch("reload scripts/equip.lua")
            mock_reload.assert_called_once_with("scripts/equip.lua")


if __name__ == "__main__":
    unittest.main()

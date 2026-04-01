"""
Tests for ai_dev/tools/git_ops.py
"""

import subprocess
import unittest
from pathlib import Path
from unittest.mock import patch, MagicMock

import sys
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from tools.git_ops import GitOps


class TestGitOps(unittest.TestCase):

    def setUp(self):
        self.repo_root = Path("/tmp/test_repo")
        self.git = GitOps(self.repo_root)

    # ------------------------------------------------------------------
    # Query commands
    # ------------------------------------------------------------------

    @patch("subprocess.run")
    def test_status(self, mock_run):
        mock_run.return_value = MagicMock(
            stdout=" M file.cpp\n?? new.py\n", stderr="", returncode=0
        )
        out, ok = self.git.status()
        self.assertTrue(ok)
        self.assertIn("file.cpp", out)
        self.assertIn("new.py", out)
        cmd_args = mock_run.call_args[0][0]
        self.assertEqual(cmd_args, ["git", "status", "--short"])

    @patch("subprocess.run")
    def test_diff(self, mock_run):
        mock_run.return_value = MagicMock(
            stdout="+int x = 1;\n-int x = 2;", stderr="", returncode=0
        )
        out, ok = self.git.diff()
        self.assertTrue(ok)
        self.assertIn("+int x = 1;", out)

    @patch("subprocess.run")
    def test_diff_staged(self, mock_run):
        mock_run.return_value = MagicMock(stdout="", stderr="", returncode=0)
        self.git.diff(staged=True)
        cmd_args = mock_run.call_args[0][0]
        self.assertIn("--cached", cmd_args)

    @patch("subprocess.run")
    def test_diff_path(self, mock_run):
        mock_run.return_value = MagicMock(stdout="", stderr="", returncode=0)
        self.git.diff(path="src/main.cpp")
        cmd_args = mock_run.call_args[0][0]
        self.assertIn("src/main.cpp", cmd_args)

    @patch("subprocess.run")
    def test_current_branch(self, mock_run):
        mock_run.return_value = MagicMock(
            stdout="main\n", stderr="", returncode=0
        )
        branch = self.git.current_branch()
        self.assertEqual(branch, "main")

    @patch("subprocess.run")
    def test_current_branch_failure(self, mock_run):
        mock_run.return_value = MagicMock(stdout="", stderr="err", returncode=1)
        branch = self.git.current_branch()
        self.assertEqual(branch, "(unknown)")

    @patch("subprocess.run")
    def test_is_clean_true(self, mock_run):
        mock_run.return_value = MagicMock(stdout="", stderr="", returncode=0)
        self.assertTrue(self.git.is_clean())

    @patch("subprocess.run")
    def test_is_clean_false(self, mock_run):
        mock_run.return_value = MagicMock(
            stdout=" M dirty.py", stderr="", returncode=0
        )
        self.assertFalse(self.git.is_clean())

    @patch("subprocess.run")
    def test_changed_files(self, mock_run):
        # Two calls: git diff --name-only, git diff --cached --name-only
        mock_run.side_effect = [
            MagicMock(stdout="a.cpp\nb.cpp\n", stderr="", returncode=0),
            MagicMock(stdout="b.cpp\nc.cpp\n", stderr="", returncode=0),
        ]
        files = self.git.changed_files()
        self.assertEqual(files, ["a.cpp", "b.cpp", "c.cpp"])

    @patch("subprocess.run")
    def test_log_oneline(self, mock_run):
        mock_run.return_value = MagicMock(
            stdout="abc123 First commit\ndef456 Second commit",
            stderr="", returncode=0,
        )
        out, ok = self.git.log_oneline(2)
        self.assertTrue(ok)
        self.assertIn("First commit", out)

    # ------------------------------------------------------------------
    # Write commands
    # ------------------------------------------------------------------

    @patch("subprocess.run")
    def test_add_all(self, mock_run):
        mock_run.return_value = MagicMock(stdout="", stderr="", returncode=0)
        _, ok = self.git.add()
        self.assertTrue(ok)
        cmd_args = mock_run.call_args[0][0]
        self.assertIn("-A", cmd_args)

    @patch("subprocess.run")
    def test_add_specific(self, mock_run):
        mock_run.return_value = MagicMock(stdout="", stderr="", returncode=0)
        _, ok = self.git.add(["file1.cpp", "file2.h"])
        self.assertTrue(ok)
        cmd_args = mock_run.call_args[0][0]
        self.assertIn("file1.cpp", cmd_args)

    @patch("subprocess.run")
    def test_commit(self, mock_run):
        mock_run.return_value = MagicMock(
            stdout="[main abc123] test commit", stderr="", returncode=0
        )
        out, ok = self.git.commit("test commit")
        self.assertTrue(ok)

    def test_commit_empty_message(self):
        out, ok = self.git.commit("")
        self.assertFalse(ok)
        self.assertIn("Empty", out)

    @patch("subprocess.run")
    def test_checkpoint(self, mock_run):
        # add then commit
        mock_run.side_effect = [
            MagicMock(stdout="", stderr="", returncode=0),
            MagicMock(stdout="committed", stderr="", returncode=0),
        ]
        out, ok = self.git.checkpoint("AI: fix build")
        self.assertTrue(ok)

    # ------------------------------------------------------------------
    # Summary
    # ------------------------------------------------------------------

    @patch("subprocess.run")
    def test_summary_for_llm(self, mock_run):
        mock_run.side_effect = [
            MagicMock(stdout="main\n", stderr="", returncode=0),
            MagicMock(stdout=" M file.cpp\n", stderr="", returncode=0),
        ]
        summary = self.git.summary_for_llm()
        self.assertIn("Branch: main", summary)
        self.assertIn("file.cpp", summary)

    # ------------------------------------------------------------------
    # Error handling
    # ------------------------------------------------------------------

    @patch("subprocess.run", side_effect=subprocess.TimeoutExpired(cmd="git", timeout=30))
    def test_timeout(self, mock_run):
        out, ok = self.git.status()
        self.assertFalse(ok)
        self.assertIn("timed out", out)

    @patch("subprocess.run", side_effect=FileNotFoundError("git"))
    def test_git_not_found(self, mock_run):
        out, ok = self.git.status()
        self.assertFalse(ok)
        self.assertIn("not found", out)


if __name__ == "__main__":
    unittest.main()

"""
NovaForge Dev AI — Git Operations (Phase 1)

Lightweight git integration for tracking AI-applied changes.
Provides status, diff, commit, branch, and log operations so the
offline AI loop can checkpoint its work without leaving the agent.
"""

import logging
import subprocess
from pathlib import Path
from typing import Optional, Tuple, List

log = logging.getLogger(__name__)


class GitOps:
    """
    Safe git operations for the AI agent loop.

    All commands run against the repository root and capture output.
    Destructive operations (commit, checkout) require explicit calls —
    nothing runs automatically.
    """

    def __init__(self, repo_root: Path):
        self.repo_root = repo_root

    # ------------------------------------------------------------------
    # Query commands (read-only)
    # ------------------------------------------------------------------

    def status(self) -> Tuple[str, bool]:
        """Return `git status --short` output."""
        return self._run(["git", "status", "--short"])

    def diff(self, staged: bool = False, path: Optional[str] = None) -> Tuple[str, bool]:
        """Return unified diff of working tree (or staged) changes."""
        cmd = ["git", "diff", "--no-color"]
        if staged:
            cmd.append("--cached")
        if path:
            cmd += ["--", path]
        return self._run(cmd)

    def log_oneline(self, count: int = 10) -> Tuple[str, bool]:
        """Return recent commit log in one-line format."""
        return self._run(["git", "log", "--oneline", f"-{count}"])

    def current_branch(self) -> str:
        """Return the name of the current branch."""
        out, ok = self._run(["git", "rev-parse", "--abbrev-ref", "HEAD"])
        return out.strip() if ok else "(unknown)"

    def is_clean(self) -> bool:
        """Return True if the working tree has no uncommitted changes."""
        out, ok = self.status()
        return ok and out.strip() == ""

    def changed_files(self) -> List[str]:
        """Return list of changed file paths (relative to repo root)."""
        out, _ = self._run(["git", "diff", "--name-only"])
        staged, _ = self._run(["git", "diff", "--cached", "--name-only"])
        files = set()
        for line in (out + "\n" + staged).splitlines():
            line = line.strip()
            if line:
                files.add(line)
        return sorted(files)

    # ------------------------------------------------------------------
    # Write commands
    # ------------------------------------------------------------------

    def add(self, paths: Optional[List[str]] = None) -> Tuple[str, bool]:
        """Stage files. If paths is None, stage all changes."""
        cmd = ["git", "add"]
        if paths:
            cmd += paths
        else:
            cmd.append("-A")
        return self._run(cmd)

    def commit(self, message: str) -> Tuple[str, bool]:
        """Create a commit with the given message."""
        if not message.strip():
            return "Empty commit message.", False
        return self._run(["git", "commit", "-m", message])

    def create_branch(self, name: str) -> Tuple[str, bool]:
        """Create and switch to a new branch."""
        return self._run(["git", "checkout", "-b", name])

    def checkout(self, target: str) -> Tuple[str, bool]:
        """Switch to a branch or restore a file."""
        return self._run(["git", "checkout", target])

    def stash(self) -> Tuple[str, bool]:
        """Stash all working-tree changes."""
        return self._run(["git", "stash"])

    def stash_pop(self) -> Tuple[str, bool]:
        """Pop the most recent stash."""
        return self._run(["git", "stash", "pop"])

    # ------------------------------------------------------------------
    # Convenience: stage + commit in one call
    # ------------------------------------------------------------------

    def checkpoint(self, message: str) -> Tuple[str, bool]:
        """Stage all changes and commit with the given message."""
        out_add, ok_add = self.add()
        if not ok_add:
            return out_add, False
        return self.commit(message)

    # ------------------------------------------------------------------
    # Summary for LLM context
    # ------------------------------------------------------------------

    def summary_for_llm(self) -> str:
        """Return a concise summary of the repo state for LLM context."""
        branch = self.current_branch()
        status_out, _ = self.status()
        lines = [
            f"Branch: {branch}",
            f"Changed files:\n{status_out}" if status_out.strip() else "Working tree clean.",
        ]
        return "\n".join(lines)

    # ------------------------------------------------------------------
    # Private
    # ------------------------------------------------------------------

    def _run(self, cmd: list, timeout: int = 30) -> Tuple[str, bool]:
        """Execute a git command and return (output, success)."""
        log.debug(f"Git: {' '.join(cmd)}")
        try:
            result = subprocess.run(
                cmd,
                cwd=str(self.repo_root),
                capture_output=True,
                text=True,
                timeout=timeout,
            )
            combined = ""
            if result.stdout:
                combined += result.stdout
            if result.stderr:
                combined += ("\n" if combined else "") + result.stderr
            return combined.strip(), result.returncode == 0
        except subprocess.TimeoutExpired:
            return f"Git command timed out after {timeout}s", False
        except FileNotFoundError:
            return "git not found on PATH", False
        except Exception as e:
            return f"Git error: {e}", False

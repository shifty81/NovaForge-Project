"""
Tests for ai_dev/tools/build_runner.py — Phase 2 additions

Tests the C# / .NET runner and GLSL directory validation methods
added in Phase 2.
"""

import os
import shutil
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch, MagicMock

import sys
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from tools.build_runner import BuildRunner


class TestDotnetBuild(unittest.TestCase):
    """Test the C# / .NET build runner."""

    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.repo_root = Path(self.tmp)
        self.runner = BuildRunner(self.repo_root)

    def tearDown(self):
        shutil.rmtree(self.tmp, ignore_errors=True)

    @patch.object(BuildRunner, '_run')
    def test_dotnet_build_default(self, mock_run):
        mock_run.return_value = ("Build succeeded.", True)
        out, ok = self.runner.run_dotnet_build()
        self.assertTrue(ok)
        self.assertIn("Build succeeded", out)
        cmd = mock_run.call_args[0][0]
        self.assertIn("dotnet", cmd)
        self.assertIn("build", cmd)
        self.assertIn("-c", cmd)
        self.assertIn("Debug", cmd)

    @patch.object(BuildRunner, '_run')
    def test_dotnet_build_release(self, mock_run):
        mock_run.return_value = ("Build succeeded.", True)
        out, ok = self.runner.run_dotnet_build(configuration="Release")
        self.assertTrue(ok)
        cmd = mock_run.call_args[0][0]
        self.assertIn("Release", cmd)

    @patch.object(BuildRunner, '_run')
    def test_dotnet_build_with_project(self, mock_run):
        mock_run.return_value = ("Build succeeded.", True)
        # Create a dummy .csproj file
        proj_dir = self.repo_root / "MyProject"
        proj_dir.mkdir()
        proj_file = proj_dir / "MyProject.csproj"
        proj_file.write_text("<Project></Project>")

        out, ok = self.runner.run_dotnet_build(str(proj_file.relative_to(self.repo_root)))
        self.assertTrue(ok)
        cmd = mock_run.call_args[0][0]
        self.assertIn("dotnet", cmd)

    @patch.object(BuildRunner, '_run')
    def test_dotnet_build_failure(self, mock_run):
        mock_run.return_value = ("error CS0001: compilation failed", False)
        out, ok = self.runner.run_dotnet_build()
        self.assertFalse(ok)
        self.assertIn("error", out)


class TestDotnetTest(unittest.TestCase):
    """Test the C# / .NET test runner."""

    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.repo_root = Path(self.tmp)
        self.runner = BuildRunner(self.repo_root)

    def tearDown(self):
        shutil.rmtree(self.tmp, ignore_errors=True)

    @patch.object(BuildRunner, '_run')
    def test_dotnet_test_default(self, mock_run):
        mock_run.return_value = ("Passed: 10", True)
        out, ok = self.runner.run_dotnet_test()
        self.assertTrue(ok)
        cmd = mock_run.call_args[0][0]
        self.assertIn("dotnet", cmd)
        self.assertIn("test", cmd)

    @patch.object(BuildRunner, '_run')
    def test_dotnet_test_with_filter(self, mock_run):
        mock_run.return_value = ("Passed: 5", True)
        out, ok = self.runner.run_dotnet_test(filter_expr="Category=Unit")
        self.assertTrue(ok)
        cmd = mock_run.call_args[0][0]
        self.assertIn("--filter", cmd)
        self.assertIn("Category=Unit", cmd)


class TestGLSLDirValidation(unittest.TestCase):
    """Test the GLSL directory validation method."""

    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.repo_root = Path(self.tmp)
        self.runner = BuildRunner(self.repo_root)

    def tearDown(self):
        shutil.rmtree(self.tmp, ignore_errors=True)

    def test_missing_shader_dir(self):
        out, ok = self.runner.validate_glsl_dir("nonexistent/dir")
        self.assertFalse(ok)
        self.assertIn("not found", out)

    def test_empty_shader_dir(self):
        shader_dir = self.repo_root / "engine" / "shaders"
        shader_dir.mkdir(parents=True)
        out, ok = self.runner.validate_glsl_dir("engine/shaders")
        self.assertTrue(ok)
        self.assertIn("No shaders found", out)

    @patch.object(BuildRunner, 'run_glsl_validate')
    def test_all_shaders_pass(self, mock_validate):
        mock_validate.return_value = ("", True)
        shader_dir = self.repo_root / "engine" / "shaders"
        shader_dir.mkdir(parents=True)
        (shader_dir / "test.vert").write_text("#version 450\nvoid main() {}")
        (shader_dir / "test.frag").write_text("#version 450\nvoid main() {}")

        out, ok = self.runner.validate_glsl_dir("engine/shaders")
        self.assertTrue(ok)
        self.assertIn("✓", out)
        self.assertEqual(mock_validate.call_count, 2)

    @patch.object(BuildRunner, 'run_glsl_validate')
    def test_shader_failure(self, mock_validate):
        mock_validate.side_effect = [
            ("", True),
            ("ERROR: line 5: syntax error", False),
        ]
        shader_dir = self.repo_root / "engine" / "shaders"
        shader_dir.mkdir(parents=True)
        (shader_dir / "good.vert").write_text("#version 450\nvoid main() {}")
        (shader_dir / "bad.frag").write_text("invalid glsl")

        out, ok = self.runner.validate_glsl_dir("engine/shaders")
        self.assertFalse(ok)
        self.assertIn("✗", out)
        self.assertIn("syntax error", out)

    @patch.object(BuildRunner, 'run_glsl_validate')
    def test_ignores_non_shader_files(self, mock_validate):
        mock_validate.return_value = ("", True)
        shader_dir = self.repo_root / "engine" / "shaders"
        shader_dir.mkdir(parents=True)
        (shader_dir / "test.vert").write_text("")
        (shader_dir / "readme.txt").write_text("not a shader")
        (shader_dir / "config.json").write_text("{}")

        out, ok = self.runner.validate_glsl_dir("engine/shaders")
        self.assertTrue(ok)
        # Should only validate .vert, not .txt or .json
        self.assertEqual(mock_validate.call_count, 1)


if __name__ == "__main__":
    unittest.main()

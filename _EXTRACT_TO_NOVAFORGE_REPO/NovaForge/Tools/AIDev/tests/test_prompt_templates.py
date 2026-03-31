"""
Tests for ai_dev/core/prompt_templates.py
"""

import unittest
from pathlib import Path

import sys
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from core.prompt_templates import PromptTemplates


class TestSubsystemDetection(unittest.TestCase):
    """Test that file paths map to the correct subsystem."""

    def setUp(self):
        self.t = PromptTemplates()

    def test_cpp_server_header(self):
        self.assertEqual(
            self.t.detect_subsystem("cpp_server/include/systems/foo_system.h"),
            "cpp_server"
        )

    def test_cpp_server_source(self):
        self.assertEqual(
            self.t.detect_subsystem("cpp_server/src/systems/foo_system.cpp"),
            "cpp_server"
        )

    def test_cpp_client(self):
        self.assertEqual(
            self.t.detect_subsystem("cpp_client/src/rendering/camera.cpp"),
            "cpp_client"
        )

    def test_engine(self):
        self.assertEqual(
            self.t.detect_subsystem("engine/src/ECS.cpp"),
            "engine"
        )

    def test_editor(self):
        self.assertEqual(
            self.t.detect_subsystem("editor/src/MainWindow.cpp"),
            "editor"
        )

    def test_python_ai_dev(self):
        self.assertEqual(
            self.t.detect_subsystem("ai_dev/tools/build_runner.py"),
            "python"
        )

    def test_blender_tool(self):
        self.assertEqual(
            self.t.detect_subsystem("tools/BlenderSpaceshipGenerator/ship_parts.py"),
            "blender"
        )

    def test_glsl_vert(self):
        self.assertEqual(
            self.t.detect_subsystem("shaders/pbr.vert"),
            "glsl"
        )

    def test_glsl_frag(self):
        self.assertEqual(
            self.t.detect_subsystem("shaders/pbr.frag"),
            "glsl"
        )

    def test_lua_script(self):
        self.assertEqual(
            self.t.detect_subsystem("scripts/equipment.lua"),
            "lua"
        )

    def test_csharp(self):
        self.assertEqual(
            self.t.detect_subsystem("tools/Editor.cs"),
            "csharp"
        )

    def test_json_data(self):
        self.assertEqual(
            self.t.detect_subsystem("data/ships.json"),
            "json"
        )

    def test_cmake(self):
        self.assertEqual(
            self.t.detect_subsystem("CMakeLists.cmake"),
            "cmake"
        )

    def test_unknown_extension(self):
        self.assertEqual(
            self.t.detect_subsystem("file.xyz"),
            ""
        )

    def test_no_extension(self):
        self.assertEqual(
            self.t.detect_subsystem("Makefile"),
            ""
        )


class TestForSubsystem(unittest.TestCase):
    """Test template retrieval by subsystem name."""

    def setUp(self):
        self.t = PromptTemplates()

    def test_cpp_server_template(self):
        tmpl = self.t.for_subsystem("cpp_server")
        self.assertIn("snake_case", tmpl)
        self.assertIn("SingleComponentSystem", tmpl)
        self.assertIn("atlas::Logger", tmpl)

    def test_cpp_client_template(self):
        tmpl = self.t.for_subsystem("cpp_client")
        self.assertIn("OpenGL", tmpl)
        self.assertIn("-Z forward", tmpl)

    def test_engine_template(self):
        tmpl = self.t.for_subsystem("engine")
        self.assertIn("PascalCase", tmpl)
        self.assertIn("m_camelCase", tmpl)

    def test_editor_template(self):
        tmpl = self.t.for_subsystem("editor")
        self.assertIn("EditorCommandBus", tmpl)
        self.assertIn("UndoableCommandBus", tmpl)

    def test_python_template(self):
        tmpl = self.t.for_subsystem("python")
        self.assertIn("Python 3.8", tmpl)
        self.assertIn("unittest", tmpl)

    def test_lua_template(self):
        tmpl = self.t.for_subsystem("lua")
        self.assertIn("dofile", tmpl)

    def test_glsl_template(self):
        tmpl = self.t.for_subsystem("glsl")
        self.assertIn("GLSL", tmpl)
        self.assertIn("glslangValidator", tmpl)

    def test_csharp_template(self):
        tmpl = self.t.for_subsystem("csharp")
        self.assertIn("dotnet", tmpl)

    def test_json_template(self):
        tmpl = self.t.for_subsystem("json")
        self.assertIn("snake_case", tmpl)

    def test_blender_template(self):
        tmpl = self.t.for_subsystem("blender")
        self.assertIn("bmesh", tmpl)
        self.assertIn("Blender 5.0", tmpl)

    def test_cmake_template(self):
        tmpl = self.t.for_subsystem("cmake")
        self.assertIn("CMake", tmpl)

    def test_unknown_subsystem_returns_empty(self):
        tmpl = self.t.for_subsystem("nonexistent")
        self.assertEqual(tmpl, "")


class TestForFile(unittest.TestCase):
    """Test template retrieval by file path."""

    def setUp(self):
        self.t = PromptTemplates()

    def test_server_file(self):
        tmpl = self.t.for_file("cpp_server/src/systems/combat_system.cpp")
        self.assertIn("snake_case", tmpl)

    def test_engine_file(self):
        tmpl = self.t.for_file("engine/src/ECS.cpp")
        self.assertIn("PascalCase", tmpl)

    def test_unknown_file(self):
        tmpl = self.t.for_file("unknown/path.xyz")
        self.assertEqual(tmpl, "")


class TestEnrichPrompt(unittest.TestCase):
    """Test prompt enrichment with template preambles."""

    def setUp(self):
        self.t = PromptTemplates()

    def test_enrich_with_subsystem(self):
        result = self.t.enrich_prompt("Fix the bug", subsystem="cpp_server")
        self.assertIn("snake_case", result)
        self.assertIn("Fix the bug", result)
        self.assertIn("---", result)

    def test_enrich_with_file_path(self):
        result = self.t.enrich_prompt(
            "Fix the bug",
            rel_path="cpp_server/src/systems/foo.cpp"
        )
        self.assertIn("snake_case", result)
        self.assertIn("Fix the bug", result)

    def test_enrich_subsystem_overrides_path(self):
        result = self.t.enrich_prompt(
            "Fix the bug",
            rel_path="cpp_server/src/foo.cpp",
            subsystem="engine"
        )
        # Subsystem should take priority
        self.assertIn("PascalCase", result)
        self.assertIn("m_camelCase", result)

    def test_enrich_no_match(self):
        result = self.t.enrich_prompt("Hello world")
        self.assertEqual(result, "Hello world")

    def test_enrich_unknown_subsystem(self):
        result = self.t.enrich_prompt("Hello", subsystem="nonexistent")
        self.assertEqual(result, "Hello")


class TestAvailableSubsystems(unittest.TestCase):

    def test_returns_sorted_list(self):
        t = PromptTemplates()
        subsystems = t.available_subsystems()
        self.assertIsInstance(subsystems, list)
        self.assertEqual(subsystems, sorted(subsystems))
        self.assertIn("cpp_server", subsystems)
        self.assertIn("engine", subsystems)
        self.assertIn("python", subsystems)
        self.assertIn("glsl", subsystems)
        self.assertIn("csharp", subsystems)
        self.assertIn("blender", subsystems)
        self.assertGreaterEqual(len(subsystems), 10)


if __name__ == "__main__":
    unittest.main()

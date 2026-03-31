"""
Tests for ai_dev/tools/ecs_scaffold.py
"""

import os
import shutil
import tempfile
import unittest
from pathlib import Path

import sys
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from tools.ecs_scaffold import ECSScaffold, _snake_to_pascal, _pascal_to_snake


class TestNamingHelpers(unittest.TestCase):

    def test_snake_to_pascal(self):
        self.assertEqual(_snake_to_pascal("fleet_debrief"), "FleetDebrief")
        self.assertEqual(_snake_to_pascal("combat"), "Combat")
        self.assertEqual(_snake_to_pascal("a_b_c"), "ABC")

    def test_pascal_to_snake(self):
        self.assertEqual(_pascal_to_snake("FleetDebrief"), "fleet_debrief")
        self.assertEqual(_pascal_to_snake("Combat"), "combat")
        self.assertEqual(_pascal_to_snake("ABCDef"), "abc_def")


class TestECSScaffold(unittest.TestCase):

    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.repo_root = Path(self.tmp)
        # Create the expected directory structure
        (self.repo_root / "cpp_server" / "include" / "systems").mkdir(parents=True)
        (self.repo_root / "cpp_server" / "include" / "components").mkdir(parents=True)
        (self.repo_root / "cpp_server" / "src" / "systems").mkdir(parents=True)
        (self.repo_root / "cpp_server" / "tests").mkdir(parents=True)
        self.scaffold = ECSScaffold(self.repo_root)

    def tearDown(self):
        shutil.rmtree(self.tmp, ignore_errors=True)

    def test_generate_dry_run(self):
        files = self.scaffold.generate("fleet_debrief", dry_run=True)
        # Should return file contents but not write them
        self.assertIn("cpp_server/include/systems/fleet_debrief_system.h", files)
        self.assertIn("cpp_server/src/systems/fleet_debrief_system.cpp", files)
        self.assertIn("cpp_server/tests/test_fleet_debrief_system.cpp", files)
        self.assertIn("__component_snippet__", files)

        # Files should not exist on disk
        self.assertFalse(
            (self.repo_root / "cpp_server/include/systems/fleet_debrief_system.h").exists()
        )

    def test_generate_writes_files(self):
        files = self.scaffold.generate("fleet_debrief")
        # Files should exist on disk
        header = self.repo_root / "cpp_server/include/systems/fleet_debrief_system.h"
        cpp = self.repo_root / "cpp_server/src/systems/fleet_debrief_system.cpp"
        test = self.repo_root / "cpp_server/tests/test_fleet_debrief_system.cpp"
        self.assertTrue(header.exists())
        self.assertTrue(cpp.exists())
        self.assertTrue(test.exists())

    def test_auto_adds_system_suffix(self):
        files = self.scaffold.generate("mining_laser", dry_run=True)
        self.assertIn("cpp_server/include/systems/mining_laser_system.h", files)

    def test_already_has_system_suffix(self):
        files = self.scaffold.generate("mining_laser_system", dry_run=True)
        self.assertIn("cpp_server/include/systems/mining_laser_system.h", files)
        # Should NOT double the suffix
        self.assertNotIn("mining_laser_system_system", str(files.keys()))

    def test_header_contains_guard(self):
        files = self.scaffold.generate("fleet_debrief", dry_run=True)
        header = files["cpp_server/include/systems/fleet_debrief_system.h"]
        self.assertIn("#ifndef NOVAFORGE_SYSTEMS_FLEET_DEBRIEF_SYSTEM_H", header)
        self.assertIn("#endif", header)

    def test_header_contains_class(self):
        files = self.scaffold.generate("fleet_debrief", dry_run=True)
        header = files["cpp_server/include/systems/fleet_debrief_system.h"]
        self.assertIn("class FleetDebriefSystem", header)
        self.assertIn("FleetDebriefState", header)

    def test_cpp_contains_implementation(self):
        files = self.scaffold.generate("fleet_debrief", dry_run=True)
        cpp = files["cpp_server/src/systems/fleet_debrief_system.cpp"]
        self.assertIn('FleetDebriefSystem::FleetDebriefSystem', cpp)
        self.assertIn("updateComponent", cpp)
        self.assertIn("initialize", cpp)
        self.assertIn("namespace atlas", cpp)
        self.assertIn("namespace systems", cpp)

    def test_test_contains_assertions(self):
        files = self.scaffold.generate("fleet_debrief", dry_run=True)
        test = files["cpp_server/tests/test_fleet_debrief_system.cpp"]
        self.assertIn("assertTrue", test)
        self.assertIn("run_fleet_debrief_system_tests", test)
        self.assertIn("testFleetDebriefInit", test)
        self.assertIn("testFleetDebriefMissing", test)

    def test_component_snippet(self):
        files = self.scaffold.generate("fleet_debrief", dry_run=True)
        snippet = files["__component_snippet__"]
        self.assertIn("FleetDebriefState", snippet)
        self.assertIn("ecs::Component", snippet)
        self.assertIn("COMPONENT_TYPE", snippet)

    def test_custom_component_file(self):
        files = self.scaffold.generate("fleet_debrief", "fleet_components.h", dry_run=True)
        header = files["cpp_server/include/systems/fleet_debrief_system.h"]
        self.assertIn('components/fleet_components.h', header)

    def test_registration_instructions(self):
        instructions = self.scaffold.registration_instructions("fleet_debrief")
        self.assertIn("CMakeLists.txt", instructions)
        self.assertIn("test_main.cpp", instructions)
        self.assertIn("fleet_debrief_system", instructions)

    def test_skip_existing_file(self):
        # Create the header file first
        header_path = self.repo_root / "cpp_server/include/systems/fleet_debrief_system.h"
        header_path.write_text("// existing")

        files = self.scaffold.generate("fleet_debrief")
        # Header should not be overwritten
        self.assertEqual(header_path.read_text(), "// existing")

    def test_list_missing_systems(self):
        # Create a header without .cpp or test
        h = self.repo_root / "cpp_server/include/systems/orphan_system.h"
        h.write_text("// orphan")

        missing = self.scaffold.list_missing_systems()
        self.assertEqual(len(missing), 1)
        self.assertEqual(missing[0]["system"], "orphan_system")
        self.assertFalse(missing[0]["has_cpp"])
        self.assertFalse(missing[0]["has_test"])

    def test_list_missing_systems_complete(self):
        # Create matching header, cpp, and test
        h = self.repo_root / "cpp_server/include/systems/complete_system.h"
        h.write_text("// header")
        cpp = self.repo_root / "cpp_server/src/systems/complete_system.cpp"
        cpp.write_text("// impl")
        test = self.repo_root / "cpp_server/tests/test_complete_system.cpp"
        test.write_text("// test")

        missing = self.scaffold.list_missing_systems()
        self.assertEqual(len(missing), 0)


class TestECSScaffoldDashNames(unittest.TestCase):

    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.repo_root = Path(self.tmp)
        (self.repo_root / "cpp_server" / "include" / "systems").mkdir(parents=True)
        (self.repo_root / "cpp_server" / "src" / "systems").mkdir(parents=True)
        (self.repo_root / "cpp_server" / "tests").mkdir(parents=True)
        self.scaffold = ECSScaffold(self.repo_root)

    def tearDown(self):
        shutil.rmtree(self.tmp, ignore_errors=True)

    def test_dash_name_converted(self):
        files = self.scaffold.generate("fleet-debrief", dry_run=True)
        self.assertIn("cpp_server/include/systems/fleet_debrief_system.h", files)


if __name__ == "__main__":
    unittest.main()

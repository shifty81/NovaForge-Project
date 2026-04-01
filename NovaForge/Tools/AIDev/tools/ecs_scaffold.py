"""
NovaForge Dev AI — ECS System Scaffolding (Phase 1)

Generates new ECS system stubs (component, header, implementation, test)
following the established NovaForge patterns.  Used by the agent loop's
'scaffold' command to create boilerplate-free new systems in one step.
"""

import re
import logging
from pathlib import Path
from typing import Optional

log = logging.getLogger(__name__)

# ---------------------------------------------------------------------------
# Naming helpers
# ---------------------------------------------------------------------------

def _snake_to_pascal(name: str) -> str:
    """Convert snake_case to PascalCase: 'fleet_debrief' → 'FleetDebrief'."""
    return "".join(word.capitalize() for word in name.split("_"))


def _pascal_to_snake(name: str) -> str:
    """Convert PascalCase to snake_case: 'FleetDebrief' → 'fleet_debrief'."""
    s1 = re.sub(r"(.)([A-Z][a-z]+)", r"\1_\2", name)
    return re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", s1).lower()


# ---------------------------------------------------------------------------
# Templates
# ---------------------------------------------------------------------------

_COMPONENT_TEMPLATE = """\
class {pascal_state} : public ecs::Component {{
public:
    float elapsed = 0.0f;
    bool active = true;
    int level = 1;
    COMPONENT_TYPE({pascal_state})
}};
"""

_HEADER_TEMPLATE = """\
#ifndef NOVAFORGE_SYSTEMS_{guard}_H
#define NOVAFORGE_SYSTEMS_{guard}_H

#include "ecs/single_component_system.h"
#include "components/{component_file}"
#include <string>

namespace atlas {{
namespace systems {{

class {system_class} : public ecs::SingleComponentSystem<components::{pascal_state}> {{
public:
    explicit {system_class}(ecs::World* world);
    ~{system_class}() override = default;

    std::string getName() const override {{ return "{system_class}"; }}

    bool initialize(const std::string& entity_id);
    bool set_active(const std::string& entity_id, bool active);
    bool is_active(const std::string& entity_id) const;
    int get_level(const std::string& entity_id) const;
    bool set_level(const std::string& entity_id, int level);
    float get_elapsed(const std::string& entity_id) const;
    bool reset(const std::string& entity_id);

protected:
    void updateComponent(ecs::Entity& entity,
                         components::{pascal_state}& comp,
                         float delta_time) override;
}};

}} // namespace systems
}} // namespace atlas

#endif // NOVAFORGE_SYSTEMS_{guard}_H
"""

_CPP_TEMPLATE = """\
#include "systems/{snake_system}.h"
#include "ecs/world.h"
#include "ecs/entity.h"

namespace atlas {{
namespace systems {{

{system_class}::{system_class}(ecs::World* world)
    : SingleComponentSystem(world) {{}}

void {system_class}::updateComponent(ecs::Entity& /*entity*/,
                                      components::{pascal_state}& comp,
                                      float delta_time) {{
    if (!comp.active) return;
    comp.elapsed += delta_time;
}}

bool {system_class}::initialize(const std::string& entity_id) {{
    auto* entity = world_->getEntity(entity_id);
    if (!entity) return false;
    if (entity->getComponent<components::{pascal_state}>()) return false;
    auto comp = std::make_unique<components::{pascal_state}>();
    entity->addComponent(std::move(comp));
    return true;
}}

bool {system_class}::set_active(const std::string& entity_id, bool active) {{
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    comp->active = active;
    return true;
}}

bool {system_class}::is_active(const std::string& entity_id) const {{
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->active : false;
}}

int {system_class}::get_level(const std::string& entity_id) const {{
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->level : 0;
}}

bool {system_class}::set_level(const std::string& entity_id, int level) {{
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (level < 1) return false;
    comp->level = level;
    return true;
}}

float {system_class}::get_elapsed(const std::string& entity_id) const {{
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->elapsed : 0.0f;
}}

bool {system_class}::reset(const std::string& entity_id) {{
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    comp->elapsed = 0.0f;
    comp->active = true;
    comp->level = 1;
    return true;
}}

}} // namespace systems
}} // namespace atlas
"""

_TEST_TEMPLATE = """\
// Tests for: {system_class}
#include "test_log.h"
#include "components/{component_file}"
#include "ecs/system.h"
#include "systems/{snake_system}.h"

using namespace atlas;

static void test{short_pascal}Init() {{
    std::cout << "\\n=== {system_class}: Init ===" << std::endl;
    ecs::World world;
    systems::{system_class} sys(&world);
    world.createEntity("e1");

    assertTrue(sys.initialize("e1"), "Init on existing entity");
    assertTrue(!sys.initialize("e1"), "Double init rejected");
    assertTrue(!sys.initialize("nonexistent"), "Init on missing entity");
}}

static void test{short_pascal}Active() {{
    std::cout << "\\n=== {system_class}: Active ===" << std::endl;
    ecs::World world;
    systems::{system_class} sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.is_active("e1"), "Default active");
    assertTrue(sys.set_active("e1", false), "Deactivate succeeds");
    assertTrue(!sys.is_active("e1"), "Now inactive");
    assertTrue(sys.set_active("e1", true), "Reactivate succeeds");
    assertTrue(sys.is_active("e1"), "Active again");

    assertTrue(!sys.is_active("missing"), "Missing entity not active");
    assertTrue(!sys.set_active("missing", true), "Set active on missing fails");
}}

static void test{short_pascal}Level() {{
    std::cout << "\\n=== {system_class}: Level ===" << std::endl;
    ecs::World world;
    systems::{system_class} sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.get_level("e1") == 1, "Default level is 1");
    assertTrue(sys.set_level("e1", 5), "Set level succeeds");
    assertTrue(sys.get_level("e1") == 5, "Level updated");
    assertTrue(!sys.set_level("e1", 0), "Level 0 rejected");
    assertTrue(!sys.set_level("e1", -1), "Negative level rejected");
    assertTrue(sys.get_level("e1") == 5, "Level unchanged after reject");

    assertTrue(sys.get_level("missing") == 0, "Missing entity level is 0");
    assertTrue(!sys.set_level("missing", 3), "Set level on missing fails");
}}

static void test{short_pascal}Update() {{
    std::cout << "\\n=== {system_class}: Update ===" << std::endl;
    ecs::World world;
    systems::{system_class} sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    sys.update(0.5f);
    assertTrue(approxEqual(sys.get_elapsed("e1"), 0.5f), "Elapsed after 0.5s");
    sys.update(0.25f);
    assertTrue(approxEqual(sys.get_elapsed("e1"), 0.75f), "Elapsed after 0.75s");

    // Inactive entities don't tick
    sys.set_active("e1", false);
    sys.update(1.0f);
    assertTrue(approxEqual(sys.get_elapsed("e1"), 0.75f), "No tick when inactive");
}}

static void test{short_pascal}Reset() {{
    std::cout << "\\n=== {system_class}: Reset ===" << std::endl;
    ecs::World world;
    systems::{system_class} sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    sys.set_level("e1", 5);
    sys.set_active("e1", false);
    sys.update(1.0f);

    assertTrue(sys.reset("e1"), "Reset succeeds");
    assertTrue(sys.is_active("e1"), "Active after reset");
    assertTrue(sys.get_level("e1") == 1, "Level 1 after reset");
    assertTrue(approxEqual(sys.get_elapsed("e1"), 0.0f), "Elapsed 0 after reset");

    assertTrue(!sys.reset("missing"), "Reset on missing fails");
}}

static void test{short_pascal}Missing() {{
    std::cout << "\\n=== {system_class}: Missing Entity ===" << std::endl;
    ecs::World world;
    systems::{system_class} sys(&world);

    assertTrue(!sys.initialize("ghost"), "Init on non-existent entity");
    assertTrue(!sys.set_active("ghost", true), "set_active on missing");
    assertTrue(!sys.is_active("ghost"), "is_active on missing");
    assertTrue(sys.get_level("ghost") == 0, "get_level on missing");
    assertTrue(!sys.set_level("ghost", 1), "set_level on missing");
    assertTrue(approxEqual(sys.get_elapsed("ghost"), 0.0f), "get_elapsed on missing");
    assertTrue(!sys.reset("ghost"), "reset on missing");
}}

void run_{snake_system}_tests() {{
    test{short_pascal}Init();
    test{short_pascal}Active();
    test{short_pascal}Level();
    test{short_pascal}Update();
    test{short_pascal}Reset();
    test{short_pascal}Missing();
}}
"""


class ECSScaffold:
    """
    Generates complete ECS system boilerplate: component, header, implementation,
    and test file, following NovaForge conventions.

    Usage:
        scaffold = ECSScaffold(repo_root)
        files = scaffold.generate("fleet_debrief_system", "fleet_components.h")
        # files is a dict of {rel_path: content}
    """

    def __init__(self, repo_root: Path):
        self.repo_root = repo_root

    def generate(
        self,
        system_name: str,
        component_file: str = "game_components.h",
        dry_run: bool = False,
    ) -> dict:
        """
        Generate scaffold files for a new ECS system.

        Args:
            system_name:    Snake-case name, e.g. 'fleet_debrief_system'.
                            The '_system' suffix is added automatically if missing.
            component_file: Which components header to use (e.g. 'fleet_components.h').
            dry_run:        If True, return file contents without writing.

        Returns:
            dict of {relative_path: file_content} for all generated files.
        """
        # Normalise the name
        snake = system_name.lower().replace("-", "_")
        if not snake.endswith("_system"):
            snake += "_system"

        # Derive naming variants
        base = snake.replace("_system", "")
        pascal_base = _snake_to_pascal(base)
        system_class = f"{pascal_base}System"
        pascal_state = f"{pascal_base}State"
        guard = snake.upper()

        ctx = {
            "snake_system": snake,
            "system_class": system_class,
            "pascal_state": pascal_state,
            "short_pascal": pascal_base,
            "guard": guard,
            "component_file": component_file,
        }

        files = {}

        # 1. Component snippet (user will paste into the component file)
        comp_path = f"cpp_server/include/components/{component_file}"
        comp_snippet = _COMPONENT_TEMPLATE.format(**ctx)
        files["__component_snippet__"] = comp_snippet

        # 2. System header
        header_path = f"cpp_server/include/systems/{snake}.h"
        files[header_path] = _HEADER_TEMPLATE.format(**ctx)

        # 3. System implementation
        cpp_path = f"cpp_server/src/systems/{snake}.cpp"
        files[cpp_path] = _CPP_TEMPLATE.format(**ctx)

        # 4. Test file
        test_path = f"cpp_server/tests/test_{snake}.cpp"
        files[test_path] = _TEST_TEMPLATE.format(**ctx)

        if not dry_run:
            for rel_path, content in files.items():
                if rel_path.startswith("__"):
                    continue  # Skip meta-keys like __component_snippet__
                full = self.repo_root / rel_path
                if full.exists():
                    log.warning(f"Skipping existing file: {rel_path}")
                    continue
                full.parent.mkdir(parents=True, exist_ok=True)
                full.write_text(content, encoding="utf-8")
                log.info(f"Created: {rel_path}")

        return files

    def registration_instructions(self, system_name: str) -> str:
        """Return CMakeLists.txt and test_main.cpp registration instructions."""
        snake = system_name.lower().replace("-", "_")
        if not snake.endswith("_system"):
            snake += "_system"

        return f"""\
After generating files, register them:

1. In cpp_server/CMakeLists.txt, add to CORE_SOURCES:
   src/systems/{snake}.cpp

2. In cpp_server/tests/test_main.cpp:
   - Add forward declaration:
     void run_{snake}_tests();
   - Add call in main():
     run_{snake}_tests();
"""

    def list_missing_systems(self) -> list:
        """Return systems that have a header but no .cpp or test."""
        systems_dir = self.repo_root / "cpp_server" / "include" / "systems"
        if not systems_dir.exists():
            return []

        missing = []
        for h in sorted(systems_dir.glob("*.h")):
            base = h.stem
            cpp = self.repo_root / "cpp_server" / "src" / "systems" / f"{base}.cpp"
            test = self.repo_root / "cpp_server" / "tests" / f"test_{base}.cpp"
            if not cpp.exists() or not test.exists():
                missing.append({
                    "system": base,
                    "has_header": True,
                    "has_cpp": cpp.exists(),
                    "has_test": test.exists(),
                })
        return missing

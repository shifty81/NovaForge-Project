// Tests for: FleetInsigniaSystem
#include "test_log.h"
#include "components/social_components.h"
#include "ecs/system.h"
#include "systems/fleet_insignia_system.h"

using namespace atlas;

static void testFleetInsigniaInit() {
    std::cout << "\n=== FleetInsignia: Init ===" << std::endl;
    ecs::World world;
    systems::FleetInsigniaSystem sys(&world);
    world.createEntity("e1");
    assertTrue(sys.initialize("e1"), "Init succeeds");
    assertTrue(sys.getInsigniaName("e1").empty(), "Default insignia name empty");
    assertTrue(sys.getPrimaryColor("e1").empty(), "Default primary color empty");
    assertTrue(sys.getSecondaryColor("e1").empty(), "Default secondary color empty");
    assertTrue(sys.getSymbol("e1").empty(), "Default symbol empty");
    assertTrue(sys.getMotto("e1").empty(), "Default motto empty");
    assertTrue(!sys.isRegistered("e1"), "Not registered initially");
    assertTrue(sys.getAchievementCount("e1") == 0, "No achievements initially");
    assertTrue(sys.getEarnedAchievementCount("e1") == 0, "No earned achievements");
    assertTrue(approxEqual(sys.getCohesionBonus("e1"), 0.0f), "Cohesion bonus 0");
    assertTrue(sys.getTotalAchievementsEarned("e1") == 0, "Total earned 0");
    assertTrue(sys.getFleetId("e1").empty(), "Fleet id empty");
    assertTrue(sys.getMaxAchievements("e1") == 20, "Default max 20");
    assertTrue(!sys.initialize("nonexistent"), "Init fails on missing entity");
}

static void testFleetInsigniaDesign() {
    std::cout << "\n=== FleetInsignia: Design ===" << std::endl;
    ecs::World world;
    systems::FleetInsigniaSystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.setInsigniaName("e1", "Iron Phoenix"), "Set name succeeds");
    assertTrue(sys.getInsigniaName("e1") == "Iron Phoenix", "Name set correctly");

    assertTrue(sys.setPrimaryColor("e1", "#FF4400"), "Set primary color");
    assertTrue(sys.getPrimaryColor("e1") == "#FF4400", "Primary color correct");

    assertTrue(sys.setSecondaryColor("e1", "#002244"), "Set secondary color");
    assertTrue(sys.getSecondaryColor("e1") == "#002244", "Secondary color correct");

    assertTrue(sys.setSymbol("e1", "phoenix"), "Set symbol");
    assertTrue(sys.getSymbol("e1") == "phoenix", "Symbol correct");

    assertTrue(sys.setMotto("e1", "Forged in fire"), "Set motto");
    assertTrue(sys.getMotto("e1") == "Forged in fire", "Motto correct");

    // Empty strings rejected
    assertTrue(!sys.setInsigniaName("e1", ""), "Empty name rejected");
    assertTrue(!sys.setPrimaryColor("e1", ""), "Empty color rejected");
    assertTrue(!sys.setSymbol("e1", ""), "Empty symbol rejected");
    assertTrue(!sys.setMotto("e1", ""), "Empty motto rejected");

    // Missing entity
    assertTrue(!sys.setInsigniaName("x", "name"), "Missing entity rejected");
    assertTrue(!sys.setPrimaryColor("x", "red"), "Missing entity rejected");
}

static void testFleetInsigniaRegister() {
    std::cout << "\n=== FleetInsignia: Register ===" << std::endl;
    ecs::World world;
    systems::FleetInsigniaSystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    // Cannot register without a name
    assertTrue(!sys.registerInsignia("e1"), "Cannot register without name");
    assertTrue(!sys.isRegistered("e1"), "Not registered after failed attempt");

    // Set name then register
    sys.setInsigniaName("e1", "Iron Phoenix");
    assertTrue(sys.registerInsignia("e1"), "Register succeeds with name");
    assertTrue(sys.isRegistered("e1"), "Is registered after success");

    // Missing entity
    assertTrue(!sys.registerInsignia("missing"), "Missing entity rejected");
    assertTrue(!sys.isRegistered("missing"), "Missing entity returns false");
}

static void testFleetInsigniaAchievements() {
    std::cout << "\n=== FleetInsignia: Achievements ===" << std::endl;
    ecs::World world;
    systems::FleetInsigniaSystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.addAchievement("e1", "a1", "First blood", 0.1f), "Add a1");
    assertTrue(sys.addAchievement("e1", "a2", "Ten missions", 0.15f), "Add a2");
    assertTrue(sys.addAchievement("e1", "a3", "Explorers award", 0.05f), "Add a3");
    assertTrue(sys.getAchievementCount("e1") == 3, "3 achievements");
    assertTrue(sys.hasAchievement("e1", "a1"), "Has a1");
    assertTrue(sys.hasAchievement("e1", "a2"), "Has a2");
    assertTrue(!sys.hasAchievement("e1", "a999"), "Does not have a999");

    // Not earned yet
    assertTrue(!sys.isAchievementEarned("e1", "a1"), "a1 not earned yet");
    assertTrue(sys.getEarnedAchievementCount("e1") == 0, "0 earned");
    assertTrue(approxEqual(sys.getCohesionBonus("e1"), 0.0f), "Cohesion still 0");

    // Earn a1
    assertTrue(sys.earnAchievement("e1", "a1"), "Earn a1");
    assertTrue(sys.isAchievementEarned("e1", "a1"), "a1 is earned");
    assertTrue(sys.getEarnedAchievementCount("e1") == 1, "1 earned");
    assertTrue(approxEqual(sys.getCohesionBonus("e1"), 0.1f), "Cohesion 0.1 after a1");
    assertTrue(sys.getTotalAchievementsEarned("e1") == 1, "Total earned = 1");

    // Earn again blocked
    assertTrue(!sys.earnAchievement("e1", "a1"), "Cannot earn again");

    // Earn a2
    assertTrue(sys.earnAchievement("e1", "a2"), "Earn a2");
    assertTrue(approxEqual(sys.getCohesionBonus("e1"), 0.25f), "Cohesion 0.25 after a2");
    assertTrue(sys.getTotalAchievementsEarned("e1") == 2, "Total earned = 2");

    // Duplicate achievement rejected
    assertTrue(!sys.addAchievement("e1", "a1", "Dup", 0.1f), "Duplicate rejected");

    // Non-existent earn rejected
    assertTrue(!sys.earnAchievement("e1", "nonexistent"), "Earn nonexistent fails");
    assertTrue(!sys.earnAchievement("missing", "a1"), "Missing entity fails");
}

static void testFleetInsigniaRemoveAchievement() {
    std::cout << "\n=== FleetInsignia: RemoveAchievement ===" << std::endl;
    ecs::World world;
    systems::FleetInsigniaSystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    sys.addAchievement("e1", "a1", "Test", 0.2f);
    sys.earnAchievement("e1", "a1");
    assertTrue(approxEqual(sys.getCohesionBonus("e1"), 0.2f), "Cohesion 0.2");

    // Remove earned achievement reduces cohesion
    assertTrue(sys.removeAchievement("e1", "a1"), "Remove a1 succeeds");
    assertTrue(!sys.hasAchievement("e1", "a1"), "a1 gone");
    assertTrue(approxEqual(sys.getCohesionBonus("e1"), 0.0f), "Cohesion back to 0");

    // Remove non-existent
    assertTrue(!sys.removeAchievement("e1", "nonexistent"), "Remove nonexistent fails");
    assertTrue(!sys.removeAchievement("missing", "a1"), "Missing entity fails");
}

static void testFleetInsigniaCapacityCap() {
    std::cout << "\n=== FleetInsignia: CapacityCap ===" << std::endl;
    ecs::World world;
    systems::FleetInsigniaSystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    sys.setMaxAchievements("e1", 3);
    assertTrue(sys.getMaxAchievements("e1") == 3, "Max set to 3");

    assertTrue(sys.addAchievement("e1", "a1", "d1", 0.1f), "Add 1");
    assertTrue(sys.addAchievement("e1", "a2", "d2", 0.1f), "Add 2");
    assertTrue(sys.addAchievement("e1", "a3", "d3", 0.1f), "Add 3");
    assertTrue(!sys.addAchievement("e1", "a4", "d4", 0.1f), "Capacity cap enforced");
    assertTrue(sys.getAchievementCount("e1") == 3, "Still 3 achievements");

    // Invalid max rejected
    assertTrue(!sys.setMaxAchievements("e1", 0), "Max 0 rejected");
    assertTrue(!sys.setMaxAchievements("missing", 5), "Missing entity rejected");
}

static void testFleetInsigniaFleetId() {
    std::cout << "\n=== FleetInsignia: FleetId ===" << std::endl;
    ecs::World world;
    systems::FleetInsigniaSystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.setFleetId("e1", "fleet_alpha"), "Set fleet id");
    assertTrue(sys.getFleetId("e1") == "fleet_alpha", "Fleet id correct");
    assertTrue(!sys.setFleetId("e1", ""), "Empty fleet id rejected");
    assertTrue(!sys.setFleetId("missing", "fleet_x"), "Missing entity rejected");
    assertTrue(sys.getFleetId("missing").empty(), "Missing entity returns empty");
}

static void testFleetInsigniaTickAdvances() {
    std::cout << "\n=== FleetInsignia: Tick ===" << std::endl;
    ecs::World world;
    systems::FleetInsigniaSystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");
    sys.setInsigniaName("e1", "Phoenix");

    sys.update(1.0f);
    sys.update(1.0f);

    // State unchanged by tick (no time-sensitive logic)
    assertTrue(sys.getInsigniaName("e1") == "Phoenix", "Name unchanged after tick");
    assertTrue(!sys.isRegistered("e1"), "Not registered after tick");
    assertTrue(sys.getAchievementCount("e1") == 0, "No achievements after tick");
}

static void testFleetInsigniaMissing() {
    std::cout << "\n=== FleetInsignia: Missing Entity ===" << std::endl;
    ecs::World world;
    systems::FleetInsigniaSystem sys(&world);

    assertTrue(!sys.initialize("missing"), "Init fails");
    assertTrue(sys.getInsigniaName("missing").empty(), "Name empty");
    assertTrue(sys.getPrimaryColor("missing").empty(), "Color empty");
    assertTrue(sys.getSecondaryColor("missing").empty(), "Color empty");
    assertTrue(sys.getSymbol("missing").empty(), "Symbol empty");
    assertTrue(sys.getMotto("missing").empty(), "Motto empty");
    assertTrue(!sys.isRegistered("missing"), "Not registered");
    assertTrue(sys.getAchievementCount("missing") == 0, "No achievements");
    assertTrue(sys.getEarnedAchievementCount("missing") == 0, "No earned");
    assertTrue(!sys.hasAchievement("missing", "a1"), "No achievement");
    assertTrue(!sys.isAchievementEarned("missing", "a1"), "Not earned");
    assertTrue(approxEqual(sys.getCohesionBonus("missing"), 0.0f), "Cohesion 0");
    assertTrue(sys.getTotalAchievementsEarned("missing") == 0, "Total 0");
    assertTrue(sys.getFleetId("missing").empty(), "Fleet id empty");
    assertTrue(sys.getMaxAchievements("missing") == 0, "Max 0");
    assertTrue(!sys.addAchievement("missing", "a1", "d", 0.1f), "Add fails");
    assertTrue(!sys.earnAchievement("missing", "a1"), "Earn fails");
    assertTrue(!sys.removeAchievement("missing", "a1"), "Remove fails");
    assertTrue(!sys.registerInsignia("missing"), "Register fails");
    assertTrue(!sys.setInsigniaName("missing", "x"), "Set name fails");
    assertTrue(!sys.setFleetId("missing", "x"), "Set fleet id fails");
}

void run_fleet_insignia_system_tests() {
    testFleetInsigniaInit();
    testFleetInsigniaDesign();
    testFleetInsigniaRegister();
    testFleetInsigniaAchievements();
    testFleetInsigniaRemoveAchievement();
    testFleetInsigniaCapacityCap();
    testFleetInsigniaFleetId();
    testFleetInsigniaTickAdvances();
    testFleetInsigniaMissing();
}

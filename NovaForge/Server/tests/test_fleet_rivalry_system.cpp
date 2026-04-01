// Tests for: FleetRivalrySystem
#include "test_log.h"
#include "components/social_components.h"
#include "ecs/system.h"
#include "systems/fleet_rivalry_system.h"

using namespace atlas;

static void testFleetRivalryInit() {
    std::cout << "\n=== FleetRivalry: Init ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);
    world.createEntity("e1");
    assertTrue(sys.initialize("e1"), "Init succeeds");
    assertTrue(sys.getRivalCount("e1") == 0, "No rivals initially");
    assertTrue(sys.getActiveRivalCount("e1") == 0, "No active rivals");
    assertTrue(sys.getVendettaCount("e1") == 0, "No vendettas");
    assertTrue(sys.getTotalRivalriesFormed("e1") == 0, "Total formed 0");
    assertTrue(sys.getTotalVendettasDeclared("e1") == 0, "Total vendettas 0");
    assertTrue(sys.getTotalRivalriesResolved("e1") == 0, "Total resolved 0");
    assertTrue(sys.getFleetId("e1").empty(), "Fleet id empty");
    assertTrue(sys.getMaxRivals("e1") == 10, "Default max 10");
    assertTrue(!sys.initialize("nonexistent"), "Init fails on missing entity");
}

static void testFleetRivalryAddRemove() {
    std::cout << "\n=== FleetRivalry: AddRemove ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.addRival("e1", "r1", "The Crimson Fleet", components::RivalryType::Territorial),
               "Add rival r1");
    assertTrue(sys.addRival("e1", "r2", "Nomad Syndicate", components::RivalryType::Economic),
               "Add rival r2");
    assertTrue(sys.getRivalCount("e1") == 2, "2 rivals");
    assertTrue(sys.hasRival("e1", "r1"), "Has r1");
    assertTrue(sys.hasRival("e1", "r2"), "Has r2");
    assertTrue(!sys.hasRival("e1", "r99"), "Does not have r99");
    assertTrue(sys.getTotalRivalriesFormed("e1") == 2, "Total formed 2");

    // Duplicate rejected
    assertTrue(!sys.addRival("e1", "r1", "Dup", components::RivalryType::Territorial),
               "Duplicate rival rejected");

    // Empty id/name rejected
    assertTrue(!sys.addRival("e1", "", "Name", components::RivalryType::Territorial),
               "Empty id rejected");
    assertTrue(!sys.addRival("e1", "rx", "", components::RivalryType::Territorial),
               "Empty name rejected");

    // Remove
    assertTrue(sys.removeRival("e1", "r1"), "Remove r1");
    assertTrue(!sys.hasRival("e1", "r1"), "r1 removed");
    assertTrue(sys.getRivalCount("e1") == 1, "1 rival remaining");

    assertTrue(!sys.removeRival("e1", "nonexistent"), "Remove nonexistent fails");
    assertTrue(!sys.addRival("missing", "r3", "X", components::RivalryType::Territorial),
               "Missing entity rejected");
}

static void testFleetRivalryEncounters() {
    std::cout << "\n=== FleetRivalry: Encounters ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");
    sys.addRival("e1", "r1", "Enemy Fleet", components::RivalryType::Personal);

    assertTrue(approxEqual(sys.getRivalIntensity("e1", "r1"), 0.0f), "Intensity 0 initially");
    assertTrue(!sys.isActiveRival("e1", "r1"), "Not active initially");

    assertTrue(sys.recordEncounter("e1", "r1", 0.2f), "Record encounter +0.2");
    assertTrue(approxEqual(sys.getRivalIntensity("e1", "r1"), 0.2f), "Intensity = 0.2");
    assertTrue(sys.getEncounterCount("e1", "r1") == 1, "1 encounter");

    assertTrue(sys.recordEncounter("e1", "r1", 0.2f), "Record encounter +0.2 again");
    assertTrue(approxEqual(sys.getRivalIntensity("e1", "r1"), 0.4f), "Intensity = 0.4");

    // Active threshold default is 0.30
    assertTrue(sys.isActiveRival("e1", "r1"), "Active at 0.4");

    // Clamped at 1.0
    sys.recordEncounter("e1", "r1", 1.0f);
    assertTrue(approxEqual(sys.getRivalIntensity("e1", "r1"), 1.0f), "Intensity capped at 1.0");

    // Invalid gain rejected
    assertTrue(!sys.recordEncounter("e1", "r1", -0.1f), "Negative gain rejected");
    assertTrue(!sys.recordEncounter("e1", "nonexistent", 0.1f), "Nonexistent rival fails");
    assertTrue(!sys.recordEncounter("missing", "r1", 0.1f), "Missing entity fails");
}

static void testFleetRivalryVictoryDefeat() {
    std::cout << "\n=== FleetRivalry: VictoryDefeat ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");
    sys.addRival("e1", "r1", "Fleet B", components::RivalryType::Territorial);

    sys.recordEncounter("e1", "r1", 0.5f);  // intensity = 0.5

    assertTrue(sys.recordVictory("e1", "r1"), "Record victory");
    assertTrue(sys.getVictoriesOver("e1", "r1") == 1, "Victories = 1");
    // Victory reduces intensity slightly
    assertTrue(sys.getRivalIntensity("e1", "r1") < 0.5f, "Intensity reduced by victory");

    assertTrue(sys.recordDefeat("e1", "r1"), "Record defeat");
    assertTrue(sys.getDefeatsBy("e1", "r1") == 1, "Defeats = 1");
    // Defeat raises intensity
    float intensity_after_defeat = sys.getRivalIntensity("e1", "r1");
    assertTrue(intensity_after_defeat > 0.0f, "Intensity raised by defeat");

    assertTrue(!sys.recordVictory("e1", "nonexistent"), "Nonexistent rival fails");
    assertTrue(!sys.recordDefeat("missing", "r1"), "Missing entity fails");
}

static void testFleetRivalryVendetta() {
    std::cout << "\n=== FleetRivalry: Vendetta ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");
    sys.addRival("e1", "r1", "Blood Enemies", components::RivalryType::Vendetta);

    // Manual vendetta declaration
    assertTrue(sys.declareVendetta("e1", "r1"), "Declare vendetta");
    assertTrue(sys.isVendetta("e1", "r1"), "Is vendetta");
    assertTrue(sys.getVendettaCount("e1") == 1, "1 vendetta");
    assertTrue(sys.getTotalVendettasDeclared("e1") == 1, "Total vendettas 1");

    // Double declaration rejected
    assertTrue(!sys.declareVendetta("e1", "r1"), "Double vendetta rejected");

    // Active even at 0 intensity because it's a vendetta
    assertTrue(sys.isActiveRival("e1", "r1"), "Vendetta is always active");

    // Vendetta doesn't decay
    sys.update(100.0f);
    assertTrue(sys.isVendetta("e1", "r1"), "Vendetta persists after tick");

    assertTrue(!sys.declareVendetta("e1", "nonexistent"), "Nonexistent rival fails");
    assertTrue(!sys.declareVendetta("missing", "r1"), "Missing entity fails");
}

static void testFleetRivalryAutoVendetta() {
    std::cout << "\n=== FleetRivalry: AutoVendetta ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");
    sys.setVendettaThreshold("e1", 0.8f);
    sys.addRival("e1", "r1", "Auto Enemy", components::RivalryType::Territorial);

    // Build up intensity past threshold
    sys.recordEncounter("e1", "r1", 0.5f);
    sys.recordEncounter("e1", "r1", 0.35f);
    assertTrue(sys.getRivalIntensity("e1", "r1") >= 0.8f, "Intensity >= threshold");
    assertTrue(sys.isVendetta("e1", "r1"), "Auto-escalated to vendetta");
    assertTrue(sys.getTotalVendettasDeclared("e1") == 1, "Auto-vendetta counted");
}

static void testFleetRivalryDecayAndResolve() {
    std::cout << "\n=== FleetRivalry: DecayResolve ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");
    sys.addRival("e1", "r1", "Old Enemy", components::RivalryType::Territorial);
    sys.recordEncounter("e1", "r1", 0.5f);
    assertTrue(approxEqual(sys.getRivalIntensity("e1", "r1"), 0.5f), "Intensity 0.5");

    // Decay over time (default rate 0.005/s)
    sys.update(10.0f);
    float after_decay = sys.getRivalIntensity("e1", "r1");
    assertTrue(after_decay < 0.5f, "Intensity decayed after tick");

    // Resolve
    assertTrue(sys.resolveRivalry("e1", "r1"), "Resolve rivalry");
    assertTrue(approxEqual(sys.getRivalIntensity("e1", "r1"), 0.0f), "Intensity 0 after resolve");
    assertTrue(!sys.isVendetta("e1", "r1"), "Not vendetta after resolve");
    assertTrue(sys.getTotalRivalriesResolved("e1") == 1, "Total resolved 1");

    assertTrue(!sys.resolveRivalry("e1", "nonexistent"), "Nonexistent fails");
    assertTrue(!sys.resolveRivalry("missing", "r1"), "Missing entity fails");
}

static void testFleetRivalryCapacityCap() {
    std::cout << "\n=== FleetRivalry: CapacityCap ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");
    sys.setMaxRivals("e1", 3);

    sys.addRival("e1", "r1", "N1", components::RivalryType::Territorial);
    sys.addRival("e1", "r2", "N2", components::RivalryType::Economic);
    sys.addRival("e1", "r3", "N3", components::RivalryType::Personal);
    assertTrue(!sys.addRival("e1", "r4", "N4", components::RivalryType::Territorial),
               "Capacity cap enforced");
    assertTrue(sys.getRivalCount("e1") == 3, "Still 3 rivals");

    assertTrue(!sys.setMaxRivals("e1", 0), "Max 0 rejected");
    assertTrue(!sys.setMaxRivals("missing", 5), "Missing entity rejected");
}

static void testFleetRivalryClearRivals() {
    std::cout << "\n=== FleetRivalry: ClearRivals ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    sys.addRival("e1", "r1", "N1", components::RivalryType::Territorial);
    sys.addRival("e1", "r2", "N2", components::RivalryType::Economic);
    assertTrue(sys.getRivalCount("e1") == 2, "2 rivals");

    assertTrue(sys.clearRivals("e1"), "Clear rivals succeeds");
    assertTrue(sys.getRivalCount("e1") == 0, "0 rivals after clear");

    assertTrue(!sys.clearRivals("missing"), "Missing entity rejected");
}

static void testFleetRivalryRivalryType() {
    std::cout << "\n=== FleetRivalry: RivalryType ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    sys.addRival("e1", "r1", "N1", components::RivalryType::Economic);
    sys.addRival("e1", "r2", "N2", components::RivalryType::Personal);

    assertTrue(sys.getRivalryType("e1", "r1") == components::RivalryType::Economic,
               "Type Economic correct");
    assertTrue(sys.getRivalryType("e1", "r2") == components::RivalryType::Personal,
               "Type Personal correct");
}

static void testFleetRivalryConfiguration() {
    std::cout << "\n=== FleetRivalry: Configuration ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.setFleetId("e1", "fleet_42"), "Set fleet id");
    assertTrue(sys.getFleetId("e1") == "fleet_42", "Fleet id correct");
    assertTrue(!sys.setFleetId("e1", ""), "Empty fleet id rejected");

    assertTrue(sys.setVendettaThreshold("e1", 0.85f), "Set vendetta threshold");
    assertTrue(sys.setActiveRivalryThreshold("e1", 0.25f), "Set active threshold");

    // Out-of-range rejected
    assertTrue(!sys.setVendettaThreshold("e1", 1.5f), "Out-of-range rejected");
    assertTrue(!sys.setVendettaThreshold("e1", -0.1f), "Negative rejected");
    assertTrue(!sys.setActiveRivalryThreshold("missing", 0.3f), "Missing entity rejected");

    // Decay rate
    sys.addRival("e1", "r1", "N1", components::RivalryType::Territorial);
    assertTrue(sys.setDecayRate("e1", "r1", 0.01f), "Set decay rate");
    assertTrue(!sys.setDecayRate("e1", "r1", -0.1f), "Negative decay rejected");
    assertTrue(!sys.setDecayRate("e1", "nonexistent", 0.01f), "Nonexistent rival rejected");
}

static void testFleetRivalryMissing() {
    std::cout << "\n=== FleetRivalry: Missing Entity ===" << std::endl;
    ecs::World world;
    systems::FleetRivalrySystem sys(&world);

    assertTrue(!sys.initialize("missing"), "Init fails");
    assertTrue(sys.getRivalCount("missing") == 0, "Count 0");
    assertTrue(!sys.hasRival("missing", "r1"), "No rival");
    assertTrue(approxEqual(sys.getRivalIntensity("missing", "r1"), 0.0f), "Intensity 0");
    assertTrue(!sys.isActiveRival("missing", "r1"), "Not active");
    assertTrue(!sys.isVendetta("missing", "r1"), "Not vendetta");
    assertTrue(sys.getEncounterCount("missing", "r1") == 0, "Encounters 0");
    assertTrue(sys.getVictoriesOver("missing", "r1") == 0, "Victories 0");
    assertTrue(sys.getDefeatsBy("missing", "r1") == 0, "Defeats 0");
    assertTrue(sys.getActiveRivalCount("missing") == 0, "Active count 0");
    assertTrue(sys.getVendettaCount("missing") == 0, "Vendetta count 0");
    assertTrue(sys.getTotalRivalriesFormed("missing") == 0, "Total formed 0");
    assertTrue(sys.getTotalVendettasDeclared("missing") == 0, "Total vendettas 0");
    assertTrue(sys.getTotalRivalriesResolved("missing") == 0, "Total resolved 0");
    assertTrue(sys.getFleetId("missing").empty(), "Fleet id empty");
    assertTrue(sys.getMaxRivals("missing") == 0, "Max rivals 0");
    assertTrue(!sys.addRival("missing", "r1", "N", components::RivalryType::Territorial),
               "Add fails");
    assertTrue(!sys.removeRival("missing", "r1"), "Remove fails");
    assertTrue(!sys.recordEncounter("missing", "r1", 0.1f), "Record encounter fails");
    assertTrue(!sys.recordVictory("missing", "r1"), "Record victory fails");
    assertTrue(!sys.recordDefeat("missing", "r1"), "Record defeat fails");
    assertTrue(!sys.declareVendetta("missing", "r1"), "Declare vendetta fails");
    assertTrue(!sys.resolveRivalry("missing", "r1"), "Resolve fails");
    assertTrue(!sys.setFleetId("missing", "x"), "Set fleet id fails");
    assertTrue(!sys.clearRivals("missing"), "Clear fails");
}

void run_fleet_rivalry_system_tests() {
    testFleetRivalryInit();
    testFleetRivalryAddRemove();
    testFleetRivalryEncounters();
    testFleetRivalryVictoryDefeat();
    testFleetRivalryVendetta();
    testFleetRivalryAutoVendetta();
    testFleetRivalryDecayAndResolve();
    testFleetRivalryCapacityCap();
    testFleetRivalryClearRivals();
    testFleetRivalryRivalryType();
    testFleetRivalryConfiguration();
    testFleetRivalryMissing();
}

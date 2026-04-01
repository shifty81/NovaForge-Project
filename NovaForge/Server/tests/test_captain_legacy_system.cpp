// Tests for: CaptainLegacySystem
#include "test_log.h"
#include "components/social_components.h"
#include "ecs/system.h"
#include "systems/captain_legacy_system.h"

using namespace atlas;

static void testCaptainLegacyInit() {
    std::cout << "\n=== CaptainLegacy: Init ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    assertTrue(sys.initialize("e1"), "Init succeeds");
    assertTrue(sys.getRank("e1") == components::LegacyRank::Rookie, "Default rank Rookie");
    assertTrue(sys.getRankName("e1") == "Rookie", "Rank name Rookie");
    assertTrue(sys.getTotalKills("e1") == 0, "Default kills 0");
    assertTrue(sys.getTotalMissions("e1") == 0, "Default missions 0");
    assertTrue(sys.getTotalDeployments("e1") == 0, "Default deployments 0");
    assertTrue(approxEqual(sys.getYearsServed("e1"), 0.0f), "Default years 0");
    assertTrue(sys.getShipsLost("e1") == 0, "Default ships lost 0");
    assertTrue(sys.getShipsCommanded("e1") == 0, "Default ships commanded 0");
    assertTrue(sys.getTitleCount("e1") == 0, "Default titles 0");
    assertTrue(sys.getNotableCount("e1") == 0, "Default notable 0");
    assertTrue(sys.getTotalTitlesEarned("e1") == 0, "Total titles earned 0");
    assertTrue(sys.getTotalNotableRecorded("e1") == 0, "Total notable 0");
    assertTrue(sys.getCaptainId("e1").empty(), "Captain id empty");
    assertTrue(!sys.initialize("nonexistent"), "Init fails on missing entity");
}

static void testCaptainLegacyKills() {
    std::cout << "\n=== CaptainLegacy: Kills ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.recordKill("e1"), "Record 1 kill (default count)");
    assertTrue(sys.getTotalKills("e1") == 1, "Kills = 1");

    assertTrue(sys.recordKill("e1", 5), "Record 5 kills");
    assertTrue(sys.getTotalKills("e1") == 6, "Kills = 6");

    // Invalid count
    assertTrue(!sys.recordKill("e1", 0), "Zero count rejected");
    assertTrue(!sys.recordKill("e1", -1), "Negative count rejected");
    assertTrue(!sys.recordKill("missing", 1), "Missing entity rejected");
}

static void testCaptainLegacyMissions() {
    std::cout << "\n=== CaptainLegacy: Missions ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.completeMission("e1"), "Complete 1 mission");
    assertTrue(sys.getTotalMissions("e1") == 1, "Missions = 1");

    assertTrue(sys.completeMission("e1", 3), "Complete 3 missions");
    assertTrue(sys.getTotalMissions("e1") == 4, "Missions = 4");

    assertTrue(!sys.completeMission("e1", 0), "Zero count rejected");
    assertTrue(!sys.completeMission("missing", 1), "Missing entity rejected");
}

static void testCaptainLegacyDeployment() {
    std::cout << "\n=== CaptainLegacy: Deployment ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.recordDeployment("e1"), "Record deployment");
    assertTrue(sys.getTotalDeployments("e1") == 1, "Deployments = 1");

    assertTrue(sys.recordDeployment("e1"), "Record 2nd deployment");
    assertTrue(sys.getTotalDeployments("e1") == 2, "Deployments = 2");

    assertTrue(!sys.recordDeployment("missing"), "Missing entity rejected");
}

static void testCaptainLegacyYearsServed() {
    std::cout << "\n=== CaptainLegacy: YearsServed ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.addYearsServed("e1", 2.5f), "Add 2.5 years");
    assertTrue(approxEqual(sys.getYearsServed("e1"), 2.5f), "Years = 2.5");

    assertTrue(sys.addYearsServed("e1", 1.0f), "Add 1 more year");
    assertTrue(approxEqual(sys.getYearsServed("e1"), 3.5f), "Years = 3.5");

    assertTrue(!sys.addYearsServed("e1", -0.1f), "Negative years rejected");
    assertTrue(!sys.addYearsServed("missing", 1.0f), "Missing entity rejected");
}

static void testCaptainLegacyShips() {
    std::cout << "\n=== CaptainLegacy: Ships ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.gainCommand("e1"), "Gain command");
    assertTrue(sys.getShipsCommanded("e1") == 1, "Ships commanded = 1");

    assertTrue(sys.loseShip("e1"), "Lose ship");
    assertTrue(sys.getShipsLost("e1") == 1, "Ships lost = 1");

    assertTrue(!sys.gainCommand("missing"), "Missing entity rejected");
    assertTrue(!sys.loseShip("missing"), "Missing entity rejected");
}

static void testCaptainLegacyRankProgression() {
    std::cout << "\n=== CaptainLegacy: RankProgression ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    // Rookie: score < 20
    assertTrue(sys.getRank("e1") == components::LegacyRank::Rookie, "Starts Rookie");

    // Veteran: score >= 20 (kills + missions*2 + deployments)
    sys.recordKill("e1", 10);  // score = 10
    sys.completeMission("e1", 5); // score = 10 + 10 = 20
    assertTrue(sys.getRank("e1") == components::LegacyRank::Veteran, "Rank Veteran at 20");
    assertTrue(sys.getRankName("e1") == "Veteran", "Rank name Veteran");

    // Elite: score >= 80
    sys.recordKill("e1", 30);   // score = 40 + 10 = 50, still < 80
    sys.completeMission("e1", 15); // +30 missions*2 = 80
    assertTrue(sys.getRank("e1") == components::LegacyRank::Elite, "Rank Elite at 80");
    assertTrue(sys.getRankName("e1") == "Elite", "Rank name Elite");

    // Legend: score >= 200
    sys.recordKill("e1", 80);   // score = 120 + 40 = 160 < 200
    sys.completeMission("e1", 20); // +40 missions*2 = 200
    assertTrue(sys.getRank("e1") == components::LegacyRank::Legend, "Rank Legend at 200");
    assertTrue(sys.getRankName("e1") == "Legend", "Rank name Legend");
}

static void testCaptainLegacyTitles() {
    std::cout << "\n=== CaptainLegacy: Titles ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.addTitle("e1", "Ace Pilot"), "Add title 1");
    assertTrue(sys.addTitle("e1", "War Hero"), "Add title 2");
    assertTrue(sys.getTitleCount("e1") == 2, "2 titles");
    assertTrue(sys.hasTitle("e1", "Ace Pilot"), "Has Ace Pilot");
    assertTrue(sys.hasTitle("e1", "War Hero"), "Has War Hero");
    assertTrue(!sys.hasTitle("e1", "Unknown"), "Does not have Unknown");
    assertTrue(sys.getTotalTitlesEarned("e1") == 2, "Total earned 2");

    // Duplicate rejected
    assertTrue(!sys.addTitle("e1", "Ace Pilot"), "Duplicate title rejected");

    // Empty title rejected
    assertTrue(!sys.addTitle("e1", ""), "Empty title rejected");

    // Remove title
    assertTrue(sys.removeTitle("e1", "War Hero"), "Remove War Hero");
    assertTrue(!sys.hasTitle("e1", "War Hero"), "War Hero removed");
    assertTrue(sys.getTitleCount("e1") == 1, "1 title remains");

    // Remove non-existent
    assertTrue(!sys.removeTitle("e1", "Unknown"), "Remove nonexistent fails");
    assertTrue(!sys.addTitle("missing", "t"), "Missing entity rejected");
    assertTrue(!sys.removeTitle("missing", "t"), "Missing entity rejected");
}

static void testCaptainLegacyTitleCap() {
    std::cout << "\n=== CaptainLegacy: TitleCap ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    sys.setMaxTitles("e1", 3);
    sys.addTitle("e1", "T1");
    sys.addTitle("e1", "T2");
    sys.addTitle("e1", "T3");
    assertTrue(!sys.addTitle("e1", "T4"), "Title cap enforced");
    assertTrue(sys.getTitleCount("e1") == 3, "Still 3 titles");

    assertTrue(!sys.setMaxTitles("e1", 0), "Max 0 rejected");
    assertTrue(!sys.setMaxTitles("missing", 5), "Missing entity rejected");
}

static void testCaptainLegacyNotableEngagements() {
    std::cout << "\n=== CaptainLegacy: NotableEngagements ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.noteEngagement("e1", "Battle of Asakai"), "Note engagement 1");
    assertTrue(sys.noteEngagement("e1", "Defence of Jita"), "Note engagement 2");
    assertTrue(sys.getNotableCount("e1") == 2, "2 notable engagements");
    assertTrue(sys.getTotalNotableRecorded("e1") == 2, "Total notable 2");

    // Empty description rejected
    assertTrue(!sys.noteEngagement("e1", ""), "Empty description rejected");
    assertTrue(!sys.noteEngagement("missing", "x"), "Missing entity rejected");
}

static void testCaptainLegacyNotableCap() {
    std::cout << "\n=== CaptainLegacy: NotableCap ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    sys.setMaxNotable("e1", 2);
    sys.noteEngagement("e1", "First");
    sys.noteEngagement("e1", "Second");
    assertTrue(!sys.noteEngagement("e1", "Third"), "Notable cap enforced");
    assertTrue(sys.getNotableCount("e1") == 2, "Still 2 notables");

    assertTrue(!sys.setMaxNotable("e1", 0), "Max 0 rejected");
    assertTrue(!sys.setMaxNotable("missing", 5), "Missing entity rejected");
}

static void testCaptainLegacyCaptainId() {
    std::cout << "\n=== CaptainLegacy: CaptainId ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);
    world.createEntity("e1");
    sys.initialize("e1");

    assertTrue(sys.setCaptainId("e1", "cap_001"), "Set captain id");
    assertTrue(sys.getCaptainId("e1") == "cap_001", "Captain id correct");
    assertTrue(!sys.setCaptainId("e1", ""), "Empty id rejected");
    assertTrue(!sys.setCaptainId("missing", "x"), "Missing entity rejected");
}

static void testCaptainLegacyMissing() {
    std::cout << "\n=== CaptainLegacy: Missing Entity ===" << std::endl;
    ecs::World world;
    systems::CaptainLegacySystem sys(&world);

    assertTrue(!sys.initialize("missing"), "Init fails");
    assertTrue(sys.getRank("missing") == components::LegacyRank::Rookie, "Rank default");
    assertTrue(sys.getRankName("missing").empty(), "Rank name empty");
    assertTrue(sys.getTotalKills("missing") == 0, "Kills 0");
    assertTrue(sys.getTotalMissions("missing") == 0, "Missions 0");
    assertTrue(sys.getTotalDeployments("missing") == 0, "Deployments 0");
    assertTrue(approxEqual(sys.getYearsServed("missing"), 0.0f), "Years 0");
    assertTrue(sys.getShipsLost("missing") == 0, "Ships lost 0");
    assertTrue(sys.getShipsCommanded("missing") == 0, "Ships commanded 0");
    assertTrue(sys.getTitleCount("missing") == 0, "Titles 0");
    assertTrue(!sys.hasTitle("missing", "t"), "No title");
    assertTrue(sys.getNotableCount("missing") == 0, "Notables 0");
    assertTrue(sys.getTotalTitlesEarned("missing") == 0, "Total titles 0");
    assertTrue(sys.getCaptainId("missing").empty(), "Captain id empty");
    assertTrue(!sys.recordKill("missing"), "Record kill fails");
    assertTrue(!sys.completeMission("missing"), "Complete mission fails");
    assertTrue(!sys.recordDeployment("missing"), "Record deployment fails");
    assertTrue(!sys.addYearsServed("missing", 1.0f), "Add years fails");
    assertTrue(!sys.loseShip("missing"), "Lose ship fails");
    assertTrue(!sys.gainCommand("missing"), "Gain command fails");
    assertTrue(!sys.addTitle("missing", "t"), "Add title fails");
    assertTrue(!sys.noteEngagement("missing", "x"), "Note engagement fails");
}

void run_captain_legacy_system_tests() {
    testCaptainLegacyInit();
    testCaptainLegacyKills();
    testCaptainLegacyMissions();
    testCaptainLegacyDeployment();
    testCaptainLegacyYearsServed();
    testCaptainLegacyShips();
    testCaptainLegacyRankProgression();
    testCaptainLegacyTitles();
    testCaptainLegacyTitleCap();
    testCaptainLegacyNotableEngagements();
    testCaptainLegacyNotableCap();
    testCaptainLegacyCaptainId();
    testCaptainLegacyMissing();
}

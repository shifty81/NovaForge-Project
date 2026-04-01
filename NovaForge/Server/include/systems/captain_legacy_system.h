#ifndef NOVAFORGE_SYSTEMS_CAPTAIN_LEGACY_SYSTEM_H
#define NOVAFORGE_SYSTEMS_CAPTAIN_LEGACY_SYSTEM_H

#include "ecs/single_component_system.h"
#include "components/social_components.h"
#include <string>

namespace atlas {
namespace systems {

// CaptainLegacySystem — Phase G (Additional Features)
// Tracks the lifetime career record of a single captain. Accumulated kills,
// missions, deployments, and years served drive a rank progression:
// Rookie → Veteran → Elite → Legend. Titles and notable engagements are
// stored for lore purposes and chatter references.
class CaptainLegacySystem
    : public ecs::SingleComponentSystem<components::CaptainLegacyState> {
public:
    explicit CaptainLegacySystem(ecs::World* world);
    ~CaptainLegacySystem() override = default;

    std::string getName() const override { return "CaptainLegacySystem"; }

    bool initialize(const std::string& entity_id);

    // Career event recording
    bool recordKill(const std::string& entity_id, int count = 1);
    bool completeMission(const std::string& entity_id, int count = 1);
    bool recordDeployment(const std::string& entity_id);
    bool addYearsServed(const std::string& entity_id, float years);
    bool loseShip(const std::string& entity_id);
    bool gainCommand(const std::string& entity_id);

    // Narrative extras
    bool addTitle(const std::string& entity_id, const std::string& title);
    bool removeTitle(const std::string& entity_id, const std::string& title);
    bool noteEngagement(const std::string& entity_id,
                        const std::string& description);

    // Configuration
    bool setCaptainId(const std::string& entity_id,
                      const std::string& captain_id);
    bool setMaxTitles(const std::string& entity_id, int max);
    bool setMaxNotable(const std::string& entity_id, int max);

    // Queries
    components::LegacyRank getRank(const std::string& entity_id) const;
    std::string            getRankName(const std::string& entity_id) const;
    int         getTotalKills(const std::string& entity_id) const;
    int         getTotalMissions(const std::string& entity_id) const;
    int         getTotalDeployments(const std::string& entity_id) const;
    float       getYearsServed(const std::string& entity_id) const;
    int         getShipsLost(const std::string& entity_id) const;
    int         getShipsCommanded(const std::string& entity_id) const;
    int         getTitleCount(const std::string& entity_id) const;
    bool        hasTitle(const std::string& entity_id,
                         const std::string& title) const;
    int         getNotableCount(const std::string& entity_id) const;
    int         getTotalTitlesEarned(const std::string& entity_id) const;
    int         getTotalNotableRecorded(const std::string& entity_id) const;
    std::string getCaptainId(const std::string& entity_id) const;

protected:
    void updateComponent(ecs::Entity& entity,
                         components::CaptainLegacyState& comp,
                         float delta_time) override;

private:
    static components::LegacyRank computeRank(
        const components::CaptainLegacyState& comp);
};

} // namespace systems
} // namespace atlas

#endif // NOVAFORGE_SYSTEMS_CAPTAIN_LEGACY_SYSTEM_H

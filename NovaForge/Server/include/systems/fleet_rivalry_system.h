#ifndef NOVAFORGE_SYSTEMS_FLEET_RIVALRY_SYSTEM_H
#define NOVAFORGE_SYSTEMS_FLEET_RIVALRY_SYSTEM_H

#include "ecs/single_component_system.h"
#include "components/social_components.h"
#include <string>

namespace atlas {
namespace systems {

// FleetRivalrySystem — Phase B/C (Fleet Personality / Fleet-as-Civilization)
// Tracks rival fleets and inter-fleet antagonism. Rivalries form through
// repeated encounters and grow with defeats; they decay naturally unless
// escalated to a vendetta. High-intensity rivalries drive tactical warnings
// and fleet chatter; vendettas never decay and block diplomatic options.
class FleetRivalrySystem
    : public ecs::SingleComponentSystem<components::FleetRivalryState> {
public:
    explicit FleetRivalrySystem(ecs::World* world);
    ~FleetRivalrySystem() override = default;

    std::string getName() const override { return "FleetRivalrySystem"; }

    bool initialize(const std::string& entity_id);

    // Rival management
    bool addRival(const std::string& entity_id,
                  const std::string& rival_id,
                  const std::string& rival_name,
                  components::RivalryType type);
    bool removeRival(const std::string& entity_id,
                     const std::string& rival_id);
    bool clearRivals(const std::string& entity_id);

    // Encounter recording
    bool recordEncounter(const std::string& entity_id,
                         const std::string& rival_id,
                         float intensity_gain);
    bool recordVictory(const std::string& entity_id,
                       const std::string& rival_id);
    bool recordDefeat(const std::string& entity_id,
                      const std::string& rival_id);

    // Rivalry lifecycle
    bool declareVendetta(const std::string& entity_id,
                         const std::string& rival_id);
    bool resolveRivalry(const std::string& entity_id,
                        const std::string& rival_id);

    // Configuration
    bool setFleetId(const std::string& entity_id, const std::string& fleet_id);
    bool setMaxRivals(const std::string& entity_id, int max);
    bool setVendettaThreshold(const std::string& entity_id, float threshold);
    bool setActiveRivalryThreshold(const std::string& entity_id, float threshold);
    bool setDecayRate(const std::string& entity_id,
                      const std::string& rival_id, float rate);

    // Queries
    int         getRivalCount(const std::string& entity_id) const;
    bool        hasRival(const std::string& entity_id,
                         const std::string& rival_id) const;
    float       getRivalIntensity(const std::string& entity_id,
                                  const std::string& rival_id) const;
    bool        isActiveRival(const std::string& entity_id,
                              const std::string& rival_id) const;
    bool        isVendetta(const std::string& entity_id,
                           const std::string& rival_id) const;
    int         getEncounterCount(const std::string& entity_id,
                                  const std::string& rival_id) const;
    int         getVictoriesOver(const std::string& entity_id,
                                 const std::string& rival_id) const;
    int         getDefeatsBy(const std::string& entity_id,
                              const std::string& rival_id) const;
    int         getActiveRivalCount(const std::string& entity_id) const;
    int         getVendettaCount(const std::string& entity_id) const;
    components::RivalryType getRivalryType(const std::string& entity_id,
                                            const std::string& rival_id) const;
    int         getTotalRivalriesFormed(const std::string& entity_id) const;
    int         getTotalVendettasDeclared(const std::string& entity_id) const;
    int         getTotalRivalriesResolved(const std::string& entity_id) const;
    std::string getFleetId(const std::string& entity_id) const;
    int         getMaxRivals(const std::string& entity_id) const;

protected:
    void updateComponent(ecs::Entity& entity,
                         components::FleetRivalryState& comp,
                         float delta_time) override;
};

} // namespace systems
} // namespace atlas

#endif // NOVAFORGE_SYSTEMS_FLEET_RIVALRY_SYSTEM_H

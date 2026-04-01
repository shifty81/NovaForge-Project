#ifndef NOVAFORGE_SYSTEMS_FLEET_INSIGNIA_SYSTEM_H
#define NOVAFORGE_SYSTEMS_FLEET_INSIGNIA_SYSTEM_H

#include "ecs/single_component_system.h"
#include "components/social_components.h"
#include <string>

namespace atlas {
namespace systems {

// FleetInsigniaSystem — Phase B/C (Fleet Personality / Fleet-as-Civilization)
// Manages fleet heraldry and identity. A fleet builds its insignia over time
// by earning achievements (combat records, milestone victories, exploration
// feats). Registered insignias provide a cohesion bonus to all fleet members
// and can be referenced in chatter and UI.
class FleetInsigniaSystem
    : public ecs::SingleComponentSystem<components::FleetInsigniaState> {
public:
    explicit FleetInsigniaSystem(ecs::World* world);
    ~FleetInsigniaSystem() override = default;

    std::string getName() const override { return "FleetInsigniaSystem"; }

    bool initialize(const std::string& entity_id);

    // Insignia design
    bool setInsigniaName(const std::string& entity_id, const std::string& name);
    bool setPrimaryColor(const std::string& entity_id, const std::string& color);
    bool setSecondaryColor(const std::string& entity_id, const std::string& color);
    bool setSymbol(const std::string& entity_id, const std::string& symbol);
    bool setMotto(const std::string& entity_id, const std::string& motto);
    bool registerInsignia(const std::string& entity_id);

    // Achievement management
    bool addAchievement(const std::string& entity_id,
                        const std::string& achievement_id,
                        const std::string& description,
                        float cohesion_value);
    bool earnAchievement(const std::string& entity_id,
                         const std::string& achievement_id);
    bool removeAchievement(const std::string& entity_id,
                           const std::string& achievement_id);

    // Fleet id
    bool setFleetId(const std::string& entity_id, const std::string& fleet_id);
    bool setMaxAchievements(const std::string& entity_id, int max);

    // Queries
    std::string getInsigniaName(const std::string& entity_id) const;
    std::string getPrimaryColor(const std::string& entity_id) const;
    std::string getSecondaryColor(const std::string& entity_id) const;
    std::string getSymbol(const std::string& entity_id) const;
    std::string getMotto(const std::string& entity_id) const;
    bool        isRegistered(const std::string& entity_id) const;
    int         getAchievementCount(const std::string& entity_id) const;
    int         getEarnedAchievementCount(const std::string& entity_id) const;
    bool        hasAchievement(const std::string& entity_id,
                               const std::string& achievement_id) const;
    bool        isAchievementEarned(const std::string& entity_id,
                                    const std::string& achievement_id) const;
    float       getCohesionBonus(const std::string& entity_id) const;
    int         getTotalAchievementsEarned(const std::string& entity_id) const;
    std::string getFleetId(const std::string& entity_id) const;
    int         getMaxAchievements(const std::string& entity_id) const;

protected:
    void updateComponent(ecs::Entity& entity,
                         components::FleetInsigniaState& comp,
                         float delta_time) override;
};

} // namespace systems
} // namespace atlas

#endif // NOVAFORGE_SYSTEMS_FLEET_INSIGNIA_SYSTEM_H

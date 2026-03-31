#include "systems/fleet_insignia_system.h"
#include "ecs/world.h"
#include "ecs/entity.h"
#include <algorithm>

namespace atlas {
namespace systems {

FleetInsigniaSystem::FleetInsigniaSystem(ecs::World* world)
    : SingleComponentSystem(world) {}

void FleetInsigniaSystem::updateComponent(
        ecs::Entity& /*entity*/,
        components::FleetInsigniaState& comp,
        float delta_time) {
    if (!comp.active) return;
    comp.elapsed += delta_time;
}

bool FleetInsigniaSystem::initialize(const std::string& entity_id) {
    auto* entity = world_->getEntity(entity_id);
    if (!entity) return false;
    auto comp = std::make_unique<components::FleetInsigniaState>();
    entity->addComponent(std::move(comp));
    return true;
}

bool FleetInsigniaSystem::setInsigniaName(const std::string& entity_id,
                                           const std::string& name) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (name.empty()) return false;
    comp->insignia_name = name;
    return true;
}

bool FleetInsigniaSystem::setPrimaryColor(const std::string& entity_id,
                                           const std::string& color) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (color.empty()) return false;
    comp->primary_color = color;
    return true;
}

bool FleetInsigniaSystem::setSecondaryColor(const std::string& entity_id,
                                             const std::string& color) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (color.empty()) return false;
    comp->secondary_color = color;
    return true;
}

bool FleetInsigniaSystem::setSymbol(const std::string& entity_id,
                                     const std::string& symbol) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (symbol.empty()) return false;
    comp->symbol_name = symbol;
    return true;
}

bool FleetInsigniaSystem::setMotto(const std::string& entity_id,
                                    const std::string& motto) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (motto.empty()) return false;
    comp->motto = motto;
    return true;
}

bool FleetInsigniaSystem::registerInsignia(const std::string& entity_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (comp->insignia_name.empty()) return false;
    comp->is_registered = true;
    return true;
}

bool FleetInsigniaSystem::addAchievement(const std::string& entity_id,
                                          const std::string& achievement_id,
                                          const std::string& description,
                                          float cohesion_value) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (achievement_id.empty()) return false;
    if (description.empty()) return false;
    if (cohesion_value < 0.0f) return false;
    // Duplicate prevention
    for (const auto& a : comp->achievements) {
        if (a.achievement_id == achievement_id) return false;
    }
    // Capacity cap
    if (static_cast<int>(comp->achievements.size()) >= comp->max_achievements) return false;
    components::InsigniaAchievement a;
    a.achievement_id  = achievement_id;
    a.description     = description;
    a.cohesion_value  = cohesion_value;
    a.is_earned       = false;
    comp->achievements.push_back(a);
    return true;
}

bool FleetInsigniaSystem::earnAchievement(const std::string& entity_id,
                                           const std::string& achievement_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    for (auto& a : comp->achievements) {
        if (a.achievement_id == achievement_id) {
            if (a.is_earned) return false; // already earned
            a.is_earned = true;
            comp->cohesion_bonus += a.cohesion_value;
            ++comp->total_achievements_earned;
            return true;
        }
    }
    return false;
}

bool FleetInsigniaSystem::removeAchievement(const std::string& entity_id,
                                             const std::string& achievement_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    auto it = std::find_if(comp->achievements.begin(), comp->achievements.end(),
        [&](const components::InsigniaAchievement& a) {
            return a.achievement_id == achievement_id;
        });
    if (it == comp->achievements.end()) return false;
    if (it->is_earned) {
        comp->cohesion_bonus -= it->cohesion_value;
        if (comp->cohesion_bonus < 0.0f) comp->cohesion_bonus = 0.0f;
    }
    comp->achievements.erase(it);
    return true;
}

bool FleetInsigniaSystem::setFleetId(const std::string& entity_id,
                                      const std::string& fleet_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (fleet_id.empty()) return false;
    comp->fleet_id = fleet_id;
    return true;
}

bool FleetInsigniaSystem::setMaxAchievements(const std::string& entity_id,
                                              int max) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (max < 1) return false;
    comp->max_achievements = max;
    return true;
}

// ── Queries ────────────────────────────────────────────────────────────────

std::string FleetInsigniaSystem::getInsigniaName(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->insignia_name : "";
}

std::string FleetInsigniaSystem::getPrimaryColor(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->primary_color : "";
}

std::string FleetInsigniaSystem::getSecondaryColor(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->secondary_color : "";
}

std::string FleetInsigniaSystem::getSymbol(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->symbol_name : "";
}

std::string FleetInsigniaSystem::getMotto(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->motto : "";
}

bool FleetInsigniaSystem::isRegistered(const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->is_registered : false;
}

int FleetInsigniaSystem::getAchievementCount(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? static_cast<int>(comp->achievements.size()) : 0;
}

int FleetInsigniaSystem::getEarnedAchievementCount(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return 0;
    int n = 0;
    for (const auto& a : comp->achievements) {
        if (a.is_earned) ++n;
    }
    return n;
}

bool FleetInsigniaSystem::hasAchievement(const std::string& entity_id,
                                          const std::string& achievement_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    for (const auto& a : comp->achievements) {
        if (a.achievement_id == achievement_id) return true;
    }
    return false;
}

bool FleetInsigniaSystem::isAchievementEarned(
        const std::string& entity_id,
        const std::string& achievement_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    for (const auto& a : comp->achievements) {
        if (a.achievement_id == achievement_id) return a.is_earned;
    }
    return false;
}

float FleetInsigniaSystem::getCohesionBonus(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->cohesion_bonus : 0.0f;
}

int FleetInsigniaSystem::getTotalAchievementsEarned(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->total_achievements_earned : 0;
}

std::string FleetInsigniaSystem::getFleetId(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->fleet_id : "";
}

int FleetInsigniaSystem::getMaxAchievements(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->max_achievements : 0;
}

} // namespace systems
} // namespace atlas

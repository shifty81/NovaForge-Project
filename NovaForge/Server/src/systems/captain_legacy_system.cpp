#include "systems/captain_legacy_system.h"
#include "ecs/world.h"
#include "ecs/entity.h"
#include <algorithm>

namespace atlas {
namespace systems {

CaptainLegacySystem::CaptainLegacySystem(ecs::World* world)
    : SingleComponentSystem(world) {}

// ── Private helpers ────────────────────────────────────────────────────────

components::LegacyRank CaptainLegacySystem::computeRank(
        const components::CaptainLegacyState& comp) {
    // Score: kills + missions*2 + deployments
    int score = comp.total_kills + comp.total_missions * 2 + comp.total_deployments;
    if (score >= 200) return components::LegacyRank::Legend;
    if (score >= 80)  return components::LegacyRank::Elite;
    if (score >= 20)  return components::LegacyRank::Veteran;
    return components::LegacyRank::Rookie;
}

void CaptainLegacySystem::updateComponent(
        ecs::Entity& /*entity*/,
        components::CaptainLegacyState& comp,
        float delta_time) {
    if (!comp.active) return;
    comp.elapsed += delta_time;
    // Recompute rank on every tick (inexpensive)
    comp.rank = computeRank(comp);
}

bool CaptainLegacySystem::initialize(const std::string& entity_id) {
    auto* entity = world_->getEntity(entity_id);
    if (!entity) return false;
    auto comp = std::make_unique<components::CaptainLegacyState>();
    entity->addComponent(std::move(comp));
    return true;
}

bool CaptainLegacySystem::recordKill(const std::string& entity_id, int count) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (count <= 0) return false;
    comp->total_kills += count;
    comp->rank = computeRank(*comp);
    return true;
}

bool CaptainLegacySystem::completeMission(const std::string& entity_id,
                                           int count) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (count <= 0) return false;
    comp->total_missions += count;
    comp->rank = computeRank(*comp);
    return true;
}

bool CaptainLegacySystem::recordDeployment(const std::string& entity_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    ++comp->total_deployments;
    comp->rank = computeRank(*comp);
    return true;
}

bool CaptainLegacySystem::addYearsServed(const std::string& entity_id,
                                          float years) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (years < 0.0f) return false;
    comp->years_served += years;
    return true;
}

bool CaptainLegacySystem::loseShip(const std::string& entity_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    ++comp->ships_lost;
    return true;
}

bool CaptainLegacySystem::gainCommand(const std::string& entity_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    ++comp->ships_commanded;
    return true;
}

bool CaptainLegacySystem::addTitle(const std::string& entity_id,
                                    const std::string& title) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (title.empty()) return false;
    // Duplicate prevention
    for (const auto& t : comp->earned_titles) {
        if (t == title) return false;
    }
    // Capacity cap
    if (static_cast<int>(comp->earned_titles.size()) >= comp->max_titles) {
        return false;
    }
    comp->earned_titles.push_back(title);
    ++comp->total_titles_earned;
    return true;
}

bool CaptainLegacySystem::removeTitle(const std::string& entity_id,
                                       const std::string& title) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    auto it = std::find(comp->earned_titles.begin(),
                        comp->earned_titles.end(), title);
    if (it == comp->earned_titles.end()) return false;
    comp->earned_titles.erase(it);
    return true;
}

bool CaptainLegacySystem::noteEngagement(const std::string& entity_id,
                                          const std::string& description) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (description.empty()) return false;
    if (static_cast<int>(comp->notable_engagements.size()) >= comp->max_notable) {
        return false;
    }
    comp->notable_engagements.push_back(description);
    ++comp->total_notable_recorded;
    return true;
}

bool CaptainLegacySystem::setCaptainId(const std::string& entity_id,
                                        const std::string& captain_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (captain_id.empty()) return false;
    comp->captain_id = captain_id;
    return true;
}

bool CaptainLegacySystem::setMaxTitles(const std::string& entity_id, int max) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (max < 1) return false;
    comp->max_titles = max;
    return true;
}

bool CaptainLegacySystem::setMaxNotable(const std::string& entity_id, int max) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (max < 1) return false;
    comp->max_notable = max;
    return true;
}

// ── Queries ────────────────────────────────────────────────────────────────

components::LegacyRank CaptainLegacySystem::getRank(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->rank : components::LegacyRank::Rookie;
}

std::string CaptainLegacySystem::getRankName(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return "";
    switch (comp->rank) {
        case components::LegacyRank::Veteran: return "Veteran";
        case components::LegacyRank::Elite:   return "Elite";
        case components::LegacyRank::Legend:  return "Legend";
        default:                              return "Rookie";
    }
}

int CaptainLegacySystem::getTotalKills(const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->total_kills : 0;
}

int CaptainLegacySystem::getTotalMissions(const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->total_missions : 0;
}

int CaptainLegacySystem::getTotalDeployments(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->total_deployments : 0;
}

float CaptainLegacySystem::getYearsServed(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->years_served : 0.0f;
}

int CaptainLegacySystem::getShipsLost(const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->ships_lost : 0;
}

int CaptainLegacySystem::getShipsCommanded(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->ships_commanded : 0;
}

int CaptainLegacySystem::getTitleCount(const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? static_cast<int>(comp->earned_titles.size()) : 0;
}

bool CaptainLegacySystem::hasTitle(const std::string& entity_id,
                                    const std::string& title) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    for (const auto& t : comp->earned_titles) {
        if (t == title) return true;
    }
    return false;
}

int CaptainLegacySystem::getNotableCount(const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? static_cast<int>(comp->notable_engagements.size()) : 0;
}

int CaptainLegacySystem::getTotalTitlesEarned(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->total_titles_earned : 0;
}

int CaptainLegacySystem::getTotalNotableRecorded(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->total_notable_recorded : 0;
}

std::string CaptainLegacySystem::getCaptainId(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->captain_id : "";
}

} // namespace systems
} // namespace atlas

#include "systems/fleet_rivalry_system.h"
#include "ecs/world.h"
#include "ecs/entity.h"
#include <algorithm>

namespace atlas {
namespace systems {

FleetRivalrySystem::FleetRivalrySystem(ecs::World* world)
    : SingleComponentSystem(world) {}

void FleetRivalrySystem::updateComponent(
        ecs::Entity& /*entity*/,
        components::FleetRivalryState& comp,
        float delta_time) {
    if (!comp.active) return;
    comp.elapsed += delta_time;
    // Passive intensity decay for non-vendetta rivals
    for (auto& r : comp.rivals) {
        if (!r.is_vendetta && r.intensity > 0.0f) {
            r.intensity -= r.decay_rate * delta_time;
            if (r.intensity < 0.0f) r.intensity = 0.0f;
        }
    }
}

bool FleetRivalrySystem::initialize(const std::string& entity_id) {
    auto* entity = world_->getEntity(entity_id);
    if (!entity) return false;
    auto comp = std::make_unique<components::FleetRivalryState>();
    entity->addComponent(std::move(comp));
    return true;
}

bool FleetRivalrySystem::addRival(const std::string& entity_id,
                                   const std::string& rival_id,
                                   const std::string& rival_name,
                                   components::RivalryType type) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (rival_id.empty()) return false;
    if (rival_name.empty()) return false;
    // Duplicate prevention
    for (const auto& r : comp->rivals) {
        if (r.rival_id == rival_id) return false;
    }
    // Capacity cap
    if (static_cast<int>(comp->rivals.size()) >= comp->max_rivals) return false;
    components::RivalEntry r;
    r.rival_id   = rival_id;
    r.rival_name = rival_name;
    r.type       = type;
    r.intensity  = 0.0f;
    comp->rivals.push_back(r);
    ++comp->total_rivalries_formed;
    return true;
}

bool FleetRivalrySystem::removeRival(const std::string& entity_id,
                                      const std::string& rival_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    auto it = std::find_if(comp->rivals.begin(), comp->rivals.end(),
        [&](const components::RivalEntry& r){ return r.rival_id == rival_id; });
    if (it == comp->rivals.end()) return false;
    comp->rivals.erase(it);
    return true;
}

bool FleetRivalrySystem::clearRivals(const std::string& entity_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    comp->rivals.clear();
    return true;
}

bool FleetRivalrySystem::recordEncounter(const std::string& entity_id,
                                          const std::string& rival_id,
                                          float intensity_gain) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (intensity_gain < 0.0f) return false;
    for (auto& r : comp->rivals) {
        if (r.rival_id == rival_id) {
            ++r.total_encounters;
            r.intensity = std::min(1.0f, r.intensity + intensity_gain);
            // Auto-escalate to vendetta if threshold crossed
            if (!r.is_vendetta && r.intensity >= comp->vendetta_threshold) {
                r.is_vendetta = true;
                ++comp->total_vendettas_declared;
            }
            return true;
        }
    }
    return false;
}

bool FleetRivalrySystem::recordVictory(const std::string& entity_id,
                                        const std::string& rival_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    for (auto& r : comp->rivals) {
        if (r.rival_id == rival_id) {
            ++r.victories_over;
            // Victory slightly reduces intensity (resolved tension)
            if (!r.is_vendetta) {
                r.intensity = std::max(0.0f, r.intensity - 0.05f);
            }
            return true;
        }
    }
    return false;
}

bool FleetRivalrySystem::recordDefeat(const std::string& entity_id,
                                       const std::string& rival_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    for (auto& r : comp->rivals) {
        if (r.rival_id == rival_id) {
            ++r.defeats_by;
            // Defeat raises intensity
            r.intensity = std::min(1.0f, r.intensity + 0.10f);
            if (!r.is_vendetta && r.intensity >= comp->vendetta_threshold) {
                r.is_vendetta = true;
                ++comp->total_vendettas_declared;
            }
            return true;
        }
    }
    return false;
}

bool FleetRivalrySystem::declareVendetta(const std::string& entity_id,
                                          const std::string& rival_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    for (auto& r : comp->rivals) {
        if (r.rival_id == rival_id) {
            if (r.is_vendetta) return false; // already a vendetta
            r.is_vendetta = true;
            ++comp->total_vendettas_declared;
            return true;
        }
    }
    return false;
}

bool FleetRivalrySystem::resolveRivalry(const std::string& entity_id,
                                         const std::string& rival_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    for (auto& r : comp->rivals) {
        if (r.rival_id == rival_id) {
            r.intensity   = 0.0f;
            r.is_vendetta = false;
            ++comp->total_rivalries_resolved;
            return true;
        }
    }
    return false;
}

bool FleetRivalrySystem::setFleetId(const std::string& entity_id,
                                     const std::string& fleet_id) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (fleet_id.empty()) return false;
    comp->fleet_id = fleet_id;
    return true;
}

bool FleetRivalrySystem::setMaxRivals(const std::string& entity_id, int max) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (max < 1) return false;
    comp->max_rivals = max;
    return true;
}

bool FleetRivalrySystem::setVendettaThreshold(const std::string& entity_id,
                                               float threshold) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (threshold < 0.0f || threshold > 1.0f) return false;
    comp->vendetta_threshold = threshold;
    return true;
}

bool FleetRivalrySystem::setActiveRivalryThreshold(const std::string& entity_id,
                                                    float threshold) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (threshold < 0.0f || threshold > 1.0f) return false;
    comp->active_rivalry_threshold = threshold;
    return true;
}

bool FleetRivalrySystem::setDecayRate(const std::string& entity_id,
                                       const std::string& rival_id,
                                       float rate) {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    if (rate < 0.0f) return false;
    for (auto& r : comp->rivals) {
        if (r.rival_id == rival_id) {
            r.decay_rate = rate;
            return true;
        }
    }
    return false;
}

// ── Queries ────────────────────────────────────────────────────────────────

int FleetRivalrySystem::getRivalCount(const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? static_cast<int>(comp->rivals.size()) : 0;
}

bool FleetRivalrySystem::hasRival(const std::string& entity_id,
                                   const std::string& rival_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    for (const auto& r : comp->rivals) {
        if (r.rival_id == rival_id) return true;
    }
    return false;
}

float FleetRivalrySystem::getRivalIntensity(const std::string& entity_id,
                                             const std::string& rival_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return 0.0f;
    for (const auto& r : comp->rivals) {
        if (r.rival_id == rival_id) return r.intensity;
    }
    return 0.0f;
}

bool FleetRivalrySystem::isActiveRival(const std::string& entity_id,
                                        const std::string& rival_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    for (const auto& r : comp->rivals) {
        if (r.rival_id == rival_id) {
            return r.intensity >= comp->active_rivalry_threshold || r.is_vendetta;
        }
    }
    return false;
}

bool FleetRivalrySystem::isVendetta(const std::string& entity_id,
                                     const std::string& rival_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return false;
    for (const auto& r : comp->rivals) {
        if (r.rival_id == rival_id) return r.is_vendetta;
    }
    return false;
}

int FleetRivalrySystem::getEncounterCount(const std::string& entity_id,
                                           const std::string& rival_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return 0;
    for (const auto& r : comp->rivals) {
        if (r.rival_id == rival_id) return r.total_encounters;
    }
    return 0;
}

int FleetRivalrySystem::getVictoriesOver(const std::string& entity_id,
                                          const std::string& rival_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return 0;
    for (const auto& r : comp->rivals) {
        if (r.rival_id == rival_id) return r.victories_over;
    }
    return 0;
}

int FleetRivalrySystem::getDefeatsBy(const std::string& entity_id,
                                      const std::string& rival_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return 0;
    for (const auto& r : comp->rivals) {
        if (r.rival_id == rival_id) return r.defeats_by;
    }
    return 0;
}

int FleetRivalrySystem::getActiveRivalCount(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return 0;
    int n = 0;
    for (const auto& r : comp->rivals) {
        if (r.intensity >= comp->active_rivalry_threshold || r.is_vendetta) ++n;
    }
    return n;
}

int FleetRivalrySystem::getVendettaCount(const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return 0;
    int n = 0;
    for (const auto& r : comp->rivals) {
        if (r.is_vendetta) ++n;
    }
    return n;
}

components::RivalryType FleetRivalrySystem::getRivalryType(
        const std::string& entity_id,
        const std::string& rival_id) const {
    auto* comp = getComponentFor(entity_id);
    if (!comp) return components::RivalryType::Territorial;
    for (const auto& r : comp->rivals) {
        if (r.rival_id == rival_id) return r.type;
    }
    return components::RivalryType::Territorial;
}

int FleetRivalrySystem::getTotalRivalriesFormed(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->total_rivalries_formed : 0;
}

int FleetRivalrySystem::getTotalVendettasDeclared(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->total_vendettas_declared : 0;
}

int FleetRivalrySystem::getTotalRivalriesResolved(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->total_rivalries_resolved : 0;
}

std::string FleetRivalrySystem::getFleetId(
        const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->fleet_id : "";
}

int FleetRivalrySystem::getMaxRivals(const std::string& entity_id) const {
    auto* comp = getComponentFor(entity_id);
    return comp ? comp->max_rivals : 0;
}

} // namespace systems
} // namespace atlas

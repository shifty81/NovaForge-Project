// FactionRegistry.cpp
#include "FactionRegistry.h"
#include <algorithm>

namespace NovaForge::Gameplay::Factions
{

void FactionRegistry::initialise() {}
void FactionRegistry::shutdown()   {}

void FactionRegistry::registerFaction(const FactionDefinition& def)
{
    factions_.push_back(def);
}

std::optional<FactionDefinition> FactionRegistry::findFaction(uint32_t id) const
{
    for (const auto& f : factions_)
        if (f.factionId == id) return f;
    return std::nullopt;
}

std::vector<FactionDefinition> FactionRegistry::listFactions() const { return factions_; }

void FactionRegistry::setStanding(uint64_t entityId, uint32_t factionId, float standing)
{
    for (auto& s : standings_)
    {
        if (s.entityId == entityId && s.factionId == factionId)
        {
            s.standing = standing;
            return;
        }
    }
    standings_.push_back({entityId, factionId, standing});
}

float FactionRegistry::getStanding(uint64_t entityId, uint32_t factionId) const
{
    for (const auto& s : standings_)
        if (s.entityId == entityId && s.factionId == factionId)
            return s.standing;
    return 0.0f;
}

bool FactionRegistry::isHostile(uint64_t entityId, uint32_t factionId) const
{
    return getStanding(entityId, factionId) < -5.0f;
}

} // namespace NovaForge::Gameplay::Factions

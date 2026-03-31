// FactionRegistry.h
// NovaForge faction registry — standings, diplomacy, security zones.

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <optional>

namespace NovaForge::Gameplay::Factions
{

enum class SecurityLevel : uint8_t { HighSec, LowSec, NullSec, WormholeSpace };

struct FactionDefinition
{
    uint32_t    factionId   = 0;
    std::string name;
    std::string description;
    SecurityLevel homeZone  = SecurityLevel::HighSec;
    bool        isPlayerFaction = false;
};

struct StandingRecord
{
    uint64_t    entityId    = 0;   ///< player or corp ID
    uint32_t    factionId   = 0;
    float       standing    = 0.0f; ///< -10.0 to +10.0
};

class FactionRegistry
{
public:
    FactionRegistry()  = default;
    ~FactionRegistry() = default;

    void initialise();
    void shutdown();

    void registerFaction(const FactionDefinition& def);
    std::optional<FactionDefinition> findFaction(uint32_t factionId) const;
    std::vector<FactionDefinition>   listFactions() const;

    void  setStanding(uint64_t entityId, uint32_t factionId, float standing);
    float getStanding(uint64_t entityId, uint32_t factionId) const;
    bool  isHostile(uint64_t entityId, uint32_t factionId) const;

private:
    std::vector<FactionDefinition> factions_;
    std::vector<StandingRecord>    standings_;
};

} // namespace NovaForge::Gameplay::Factions

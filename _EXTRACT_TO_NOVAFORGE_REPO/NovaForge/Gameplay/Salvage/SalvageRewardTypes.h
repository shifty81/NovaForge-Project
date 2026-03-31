// SalvageRewardTypes.h
// NovaForge salvage — data types for salvage results and loot payouts.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Salvage
{

struct LootEntry
{
    std::string itemId;
    uint32_t    minQuantity = 1;
    uint32_t    maxQuantity = 1;
    float       dropChance  = 1.0f; ///< 0-1 probability
};

struct LootTable
{
    std::string             tableId;
    std::vector<LootEntry>  entries;
};

struct SalvageReward
{
    std::string              sourceId;   ///< Wreck / debris entity ID
    std::vector<LootEntry>   resolvedItems; ///< Entries with quantity set to resolved amount
    float                    creditsAwarded = 0.0f;
    bool                     missionProgress = false;
};

} // namespace NovaForge::Gameplay::Salvage

// LootResolver.h
// NovaForge economy — loot-table resolution, reward calculation, and inventory hookup.

#pragma once
#include "Salvage/SalvageRewardTypes.h"
#include "Inventory/InventorySystem.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Economy
{

/// Resolved reward entry after probability rolls.
struct ResolvedLootEntry
{
    std::string itemId;
    uint32_t    quantity = 0;
};

/// Full resolved reward payload from a single loot-table roll.
struct LootReward
{
    std::string                   sourceId;    ///< wreck / node / mission ID
    std::vector<ResolvedLootEntry> items;
    float                          creditsAwarded = 0.f;
    bool                           missionProgress = false;
};

class LootResolver
{
public:
    LootResolver()  = default;
    ~LootResolver() = default;

    void initialise();
    void shutdown();

    // ---- loot table registration -----------------------------------
    void registerTable(const NovaForge::Gameplay::Salvage::LootTable& table);
    bool hasTable(const std::string& tableId) const;

    // ---- resolution ------------------------------------------------
    /// Roll the loot table using the given random seed.
    std::optional<LootReward> resolve(const std::string& tableId,
                                       float creditMultiplier = 1.0f,
                                       uint64_t seed = 0) const;

    // ---- inventory hookup ------------------------------------------
    /// Insert all resolved items into the player's inventory.
    /// Returns the count of item types successfully inserted.
    int32_t applyToInventory(const LootReward& reward,
                              uint64_t playerId,
                              Inventory::InventorySystem& inventory) const;

    // ---- mining specialisation -------------------------------------
    /// Resolve a mining-extraction reward into inventory.
    int32_t applyMiningYield(uint64_t playerId,
                              const std::string& resourceType,
                              float unitsExtracted,
                              Inventory::InventorySystem& inventory) const;

private:
    std::vector<NovaForge::Gameplay::Salvage::LootTable> tables_;
};

} // namespace NovaForge::Gameplay::Economy

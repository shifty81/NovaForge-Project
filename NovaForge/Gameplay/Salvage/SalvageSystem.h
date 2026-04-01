// SalvageSystem.h
// NovaForge salvage — scanning wrecks, resolving loot tables, inserting rewards.

#pragma once
#include "Salvage/SalvageRewardTypes.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Salvage
{

struct WreckRecord
{
    uint64_t    wreckId     = 0;
    std::string wreckType;
    std::string lootTableId;
    bool        isSalvaged  = false;
};

class SalvageSystem
{
public:
    SalvageSystem()  = default;
    ~SalvageSystem() = default;

    void initialise();
    void shutdown();

    /// Register a wreck entity in the simulation.
    uint64_t spawnWreck(const std::string& wreckType, const std::string& lootTableId);

    /// Register a loot table definition.
    void registerLootTable(const LootTable& table);

    /// Perform a salvage operation on the given wreck.
    /// Returns the resolved reward and marks the wreck as salvaged.
    std::optional<SalvageReward> salvage(uint64_t wreckId);

    /// Query whether a wreck has already been salvaged.
    bool isSalvaged(uint64_t wreckId) const;

    /// Insert the resolved items from a reward into the player's inventory.
    /// Returns true when all items were successfully inserted.
    bool applyRewardToInventory(uint64_t playerId, const SalvageReward& reward);

    /// Notify mission system of progress if the reward has a mission hook.
    void notifyMissionProgress(uint64_t playerId, const SalvageReward& reward);

private:
    std::vector<WreckRecord> wrecks_;
    std::vector<LootTable>   lootTables_;
    uint64_t nextWreckId_ = 1;

    const LootTable* findTable(const std::string& tableId) const;
    SalvageReward    resolveLoot(const WreckRecord& wreck) const;
};

} // namespace NovaForge::Gameplay::Salvage

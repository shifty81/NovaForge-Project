// SalvageSystem.cpp
// NovaForge salvage — scanning wrecks, resolving loot tables, inserting rewards.

#include "Salvage/SalvageSystem.h"

#include <algorithm>
#include <cstdlib>

namespace NovaForge::Gameplay::Salvage
{

void SalvageSystem::initialise() {}
void SalvageSystem::shutdown()   { wrecks_.clear(); lootTables_.clear(); }

uint64_t SalvageSystem::spawnWreck(const std::string& wreckType,
                                   const std::string& lootTableId)
{
    WreckRecord rec;
    rec.wreckId     = nextWreckId_++;
    rec.wreckType   = wreckType;
    rec.lootTableId = lootTableId;
    rec.isSalvaged  = false;
    wrecks_.push_back(rec);
    return rec.wreckId;
}

void SalvageSystem::registerLootTable(const LootTable& table)
{
    lootTables_.push_back(table);
}

std::optional<SalvageReward> SalvageSystem::salvage(uint64_t wreckId)
{
    for (auto& w : wrecks_)
    {
        if (w.wreckId == wreckId)
        {
            if (w.isSalvaged) return std::nullopt;
            w.isSalvaged = true;
            SalvageReward reward = resolveLoot(w);
            return reward;
        }
    }
    return std::nullopt;
}

bool SalvageSystem::isSalvaged(uint64_t wreckId) const
{
    for (const auto& w : wrecks_)
        if (w.wreckId == wreckId) return w.isSalvaged;
    return false;
}

bool SalvageSystem::applyRewardToInventory(uint64_t playerId,
                                           const SalvageReward& reward)
{
    // Stub: route resolved items into the player inventory system.
    (void)playerId;
    (void)reward;
    return true;
}

void SalvageSystem::notifyMissionProgress(uint64_t playerId,
                                          const SalvageReward& reward)
{
    // Stub: if reward has a mission hook, call the MissionRegistry.
    (void)playerId;
    (void)reward;
}

// ---- private helpers --------------------------------------------------------

const LootTable* SalvageSystem::findTable(const std::string& tableId) const
{
    for (const auto& t : lootTables_)
        if (t.tableId == tableId) return &t;
    return nullptr;
}

SalvageReward SalvageSystem::resolveLoot(const WreckRecord& wreck) const
{
    SalvageReward reward;
    reward.sourceId = std::to_string(wreck.wreckId);

    const LootTable* table = findTable(wreck.lootTableId);
    if (!table) return reward;

    for (const auto& entry : table->entries)
    {
        // Deterministic resolution: include entries at or above 50% chance.
        if (entry.dropChance >= 0.5f)
        {
            LootEntry resolved = entry;
            // Use the minimum guaranteed quantity as the resolved amount.
            // A full implementation would sample uniformly in [minQuantity, maxQuantity].
            resolved.maxQuantity = resolved.minQuantity;
            reward.resolvedItems.push_back(resolved);
        }
    }

    return reward;
}

} // namespace NovaForge::Gameplay::Salvage

// LootResolver.cpp
// NovaForge economy — loot-table resolution, reward calculation, and inventory hookup.

#include "Economy/LootResolver.h"

#include <algorithm>
#include <cstdlib>

namespace NovaForge::Gameplay::Economy
{

void LootResolver::initialise() {}
void LootResolver::shutdown()   { tables_.clear(); }

void LootResolver::registerTable(const NovaForge::Gameplay::Salvage::LootTable& table)
{
    tables_.push_back(table);
}

bool LootResolver::hasTable(const std::string& tableId) const
{
    for (const auto& t : tables_)
        if (t.tableId == tableId) return true;
    return false;
}

std::optional<LootReward> LootResolver::resolve(const std::string& tableId,
                                                  float creditMultiplier,
                                                  uint64_t seed) const
{
    for (const auto& t : tables_)
    {
        if (t.tableId != tableId) continue;

        LootReward reward;
        reward.sourceId = tableId;

        // Simple LCG RNG seeded from parameter.
        uint64_t rng = (seed == 0) ? 0xDEADBEEF12345678ULL : seed;
        auto nextFloat = [&]() -> float {
            rng = rng * 6364136223846793005ULL + 1442695040888963407ULL;
            return static_cast<float>((rng >> 33) & 0x7FFFFFFF) /
                   static_cast<float>(0x7FFFFFFF);
        };

        for (const auto& entry : t.entries)
        {
            float roll = nextFloat();
            if (roll > entry.dropChance) continue;

            uint32_t qty = entry.minQuantity;
            if (entry.maxQuantity > entry.minQuantity)
            {
                float qRoll = nextFloat();
                qty = entry.minQuantity + static_cast<uint32_t>(
                    qRoll * static_cast<float>(entry.maxQuantity - entry.minQuantity + 1));
            }

            if (qty > 0)
                reward.items.push_back({ entry.itemId, qty });

            reward.creditsAwarded += static_cast<float>(qty) * 10.f * creditMultiplier;
        }

        return reward;
    }
    return std::nullopt;
}

int32_t LootResolver::applyToInventory(const LootReward& reward,
                                         uint64_t playerId,
                                         Inventory::InventorySystem& inventory) const
{
    int32_t inserted = 0;
    for (const auto& item : reward.items)
    {
        uint32_t qty = inventory.insertItems(playerId, item.itemId, item.quantity);
        if (qty > 0) ++inserted;
    }
    return inserted;
}

int32_t LootResolver::applyMiningYield(uint64_t playerId,
                                         const std::string& resourceType,
                                         float unitsExtracted,
                                         Inventory::InventorySystem& inventory) const
{
    uint32_t qty = static_cast<uint32_t>(unitsExtracted);
    if (qty == 0) return 0;
    uint32_t inserted = inventory.insertItems(playerId, resourceType, qty);
    return (inserted > 0) ? 1 : 0;
}

} // namespace NovaForge::Gameplay::Economy

// InventorySystem.cpp
// NovaForge inventory — per-entity item storage with capacity tracking.

#include "Inventory/InventorySystem.h"

#include <algorithm>

namespace NovaForge::Gameplay::Inventory
{

void InventorySystem::initialise() {}
void InventorySystem::shutdown()   { inventories_.clear(); }

void InventorySystem::registerItem(const ItemDefinition& def)
{
    itemDefs_[def.itemId] = def;
}

std::optional<ItemDefinition>
InventorySystem::findItemDef(const std::string& itemId) const
{
    auto it = itemDefs_.find(itemId);
    if (it == itemDefs_.end()) return std::nullopt;
    return it->second;
}

void InventorySystem::createInventory(uint64_t ownerId,
                                       const InventoryLimits& limits)
{
    EntityInventory inv;
    inv.ownerId = ownerId;
    inv.limits  = limits;
    inventories_[ownerId] = inv;
}

void InventorySystem::destroyInventory(uint64_t ownerId)
{
    inventories_.erase(ownerId);
}

uint32_t InventorySystem::insertItems(uint64_t ownerId,
                                       const std::string& itemId,
                                       uint32_t quantity)
{
    EntityInventory* inv = getMutable(ownerId);
    if (!inv || quantity == 0) return 0;

    // Determine how much we can actually add based on capacity.
    uint32_t canInsertQty = quantity;

    auto defOpt = findItemDef(itemId);
    if (defOpt)
    {
        const ItemDefinition& def = *defOpt;

        float remMass   = inv->limits.maxMass   - inv->currentMass;
        float remVolume = inv->limits.maxVolume - inv->currentVolume;

        if (def.massPerUnit > 0.f)
            canInsertQty = std::min(canInsertQty,
                static_cast<uint32_t>(remMass   / def.massPerUnit));
        if (def.volumePerUnit > 0.f)
            canInsertQty = std::min(canInsertQty,
                static_cast<uint32_t>(remVolume / def.volumePerUnit));

        inv->currentMass   += def.massPerUnit   * static_cast<float>(canInsertQty);
        inv->currentVolume += def.volumePerUnit * static_cast<float>(canInsertQty);
    }

    if (canInsertQty == 0) return 0;

    // Find or create a slot.
    for (auto& slot : inv->slots)
    {
        if (slot.itemId == itemId)
        {
            slot.quantity += canInsertQty;
            return canInsertQty;
        }
    }
    // New slot.
    if (inv->slots.size() < inv->limits.maxSlots)
    {
        inv->slots.push_back({ itemId, canInsertQty });
        return canInsertQty;
    }
    return 0; // no slot space
}

bool InventorySystem::removeItems(uint64_t ownerId,
                                   const std::string& itemId,
                                   uint32_t quantity)
{
    EntityInventory* inv = getMutable(ownerId);
    if (!inv) return false;

    for (auto& slot : inv->slots)
    {
        if (slot.itemId == itemId)
        {
            if (slot.quantity < quantity) return false;
            slot.quantity -= quantity;

            auto defOpt = findItemDef(itemId);
            if (defOpt)
            {
                inv->currentMass   -= defOpt->massPerUnit   * static_cast<float>(quantity);
                inv->currentVolume -= defOpt->volumePerUnit * static_cast<float>(quantity);
                if (inv->currentMass   < 0.f) inv->currentMass   = 0.f;
                if (inv->currentVolume < 0.f) inv->currentVolume = 0.f;
            }
            return true;
        }
    }
    return false;
}

uint32_t InventorySystem::countItems(uint64_t ownerId,
                                      const std::string& itemId) const
{
    auto it = inventories_.find(ownerId);
    if (it == inventories_.end()) return 0;
    for (const auto& slot : it->second.slots)
        if (slot.itemId == itemId) return slot.quantity;
    return 0;
}

bool InventorySystem::hasItems(uint64_t ownerId,
                                const std::string& itemId,
                                uint32_t quantity) const
{
    return countItems(ownerId, itemId) >= quantity;
}

std::optional<const EntityInventory*>
InventorySystem::getInventory(uint64_t ownerId) const
{
    auto it = inventories_.find(ownerId);
    if (it == inventories_.end()) return std::nullopt;
    return &it->second;
}

bool InventorySystem::canInsert(uint64_t ownerId,
                                 const std::string& itemId,
                                 uint32_t quantity) const
{
    auto it = inventories_.find(ownerId);
    if (it == inventories_.end()) return false;
    const EntityInventory& inv = it->second;

    auto defOpt = findItemDef(itemId);
    if (defOpt)
    {
        float massNeeded   = defOpt->massPerUnit   * static_cast<float>(quantity);
        float volumeNeeded = defOpt->volumePerUnit * static_cast<float>(quantity);
        if (inv.currentMass   + massNeeded   > inv.limits.maxMass)   return false;
        if (inv.currentVolume + volumeNeeded > inv.limits.maxVolume)  return false;
    }

    // Check slot availability.
    for (const auto& slot : inv.slots)
        if (slot.itemId == itemId) return true;
    return inv.slots.size() < inv.limits.maxSlots;
}

float InventorySystem::getRemainingMass(uint64_t ownerId) const
{
    auto it = inventories_.find(ownerId);
    if (it == inventories_.end()) return 0.f;
    return it->second.limits.maxMass - it->second.currentMass;
}

float InventorySystem::getRemainingVolume(uint64_t ownerId) const
{
    auto it = inventories_.find(ownerId);
    if (it == inventories_.end()) return 0.f;
    return it->second.limits.maxVolume - it->second.currentVolume;
}

EntityInventory* InventorySystem::getMutable(uint64_t ownerId)
{
    auto it = inventories_.find(ownerId);
    return (it != inventories_.end()) ? &it->second : nullptr;
}

} // namespace NovaForge::Gameplay::Inventory

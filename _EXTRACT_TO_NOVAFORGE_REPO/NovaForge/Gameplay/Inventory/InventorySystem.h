// InventorySystem.h
// NovaForge inventory — per-entity item storage with capacity tracking.

#pragma once
#include "Inventory/InventoryTypes.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace NovaForge::Gameplay::Inventory
{

/// One entity's inventory.
struct EntityInventory
{
    uint64_t               ownerId = 0;
    std::vector<InventorySlot> slots;
    InventoryLimits        limits;
    float                  currentMass   = 0.f;
    float                  currentVolume = 0.f;
};

class InventorySystem
{
public:
    InventorySystem()  = default;
    ~InventorySystem() = default;

    void initialise();
    void shutdown();

    // ---- item definitions ------------------------------------------
    void registerItem(const ItemDefinition& def);
    std::optional<ItemDefinition> findItemDef(const std::string& itemId) const;

    // ---- inventory management --------------------------------------
    void   createInventory(uint64_t ownerId, const InventoryLimits& limits = {});
    void   destroyInventory(uint64_t ownerId);

    // ---- insert / remove -------------------------------------------
    /// Insert items; returns quantity actually inserted (may be less if full).
    uint32_t insertItems(uint64_t ownerId,
                         const std::string& itemId,
                         uint32_t quantity);

    /// Remove items; returns true if the full quantity was available.
    bool removeItems(uint64_t ownerId,
                     const std::string& itemId,
                     uint32_t quantity);

    // ---- queries ---------------------------------------------------
    uint32_t countItems(uint64_t ownerId,
                        const std::string& itemId) const;

    bool hasItems(uint64_t ownerId,
                  const std::string& itemId,
                  uint32_t quantity) const;

    std::optional<const EntityInventory*> getInventory(uint64_t ownerId) const;

    // ---- capacity --------------------------------------------------
    bool canInsert(uint64_t ownerId,
                   const std::string& itemId,
                   uint32_t quantity) const;

    float getRemainingMass  (uint64_t ownerId) const;
    float getRemainingVolume(uint64_t ownerId) const;

private:
    std::unordered_map<std::string, ItemDefinition>   itemDefs_;
    std::unordered_map<uint64_t, EntityInventory>     inventories_;

    EntityInventory* getMutable(uint64_t ownerId);
};

} // namespace NovaForge::Gameplay::Inventory

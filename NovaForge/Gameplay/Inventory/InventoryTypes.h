// InventoryTypes.h
// NovaForge inventory — item, slot, and inventory data types.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Inventory
{

enum class EItemCategory : uint8_t
{
    Resource,       ///< raw ore, gas, fuel
    Component,      ///< manufactured parts
    Weapon,         ///< ship or character weapons
    Module,         ///< ship modules
    Consumable,     ///< repairs, ammo, boosts
    QuestItem,      ///< mission-specific
    Equipment,      ///< character gear
    Blueprint,      ///< manufacturing recipe unlocks
    Misc
};

struct ItemDefinition
{
    std::string      itemId;
    std::string      displayName;
    EItemCategory    category      = EItemCategory::Misc;
    float            massPerUnit   = 0.1f;   ///< kg
    float            volumePerUnit = 0.01f;  ///< m³
    bool             isStackable   = true;
    uint32_t         maxStackSize  = 9999;
    float            baseValue     = 1.0f;   ///< credits per unit
};

struct InventorySlot
{
    std::string itemId;
    uint32_t    quantity  = 0;
    bool        isEmpty() const { return quantity == 0; }
};

struct InventoryLimits
{
    float    maxMass    = 1000.0f;  ///< kg
    float    maxVolume  = 100.0f;   ///< m³
    uint32_t maxSlots   = 200;
};

} // namespace NovaForge::Gameplay::Inventory

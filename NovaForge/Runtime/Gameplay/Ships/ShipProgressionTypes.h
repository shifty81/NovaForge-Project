#pragma once

#include <string>
#include <vector>

struct ShipUpgradeSlot
{
    std::string SlotId;
    std::string Category;
    int Tier = 1;
};

struct ShipProgressionState
{
    std::string ShipId;
    std::string DisplayName;
    std::string Role;
    int HullTier = 1;
    int UtilityTier = 1;
    int CargoTier = 1;
    std::vector<ShipUpgradeSlot> UpgradeSlots;
};

#include "Gameplay/Ships/ShipProgressionSystem.h"
#include <iostream>

bool ShipProgressionSystem::Initialize()
{
    std::cout << "[ShipProgression] Initialize\n";
    return true;
}

void ShipProgressionSystem::RegisterShip(const ShipProgressionState& Ship)
{
    Ships.push_back(Ship);
    std::cout << "[ShipProgression] Registered " << Ship.ShipId << "\n";
}

ShipProgressionState* ShipProgressionSystem::FindShipMutable(const std::string& ShipId)
{
    for (auto& Ship : Ships)
    {
        if (Ship.ShipId == ShipId)
        {
            return &Ship;
        }
    }
    return nullptr;
}

const ShipProgressionState* ShipProgressionSystem::FindShip(const std::string& ShipId) const
{
    for (const auto& Ship : Ships)
    {
        if (Ship.ShipId == ShipId)
        {
            return &Ship;
        }
    }
    return nullptr;
}

void ShipProgressionSystem::UpgradeHullTier(const std::string& ShipId)
{
    if (auto* Ship = FindShipMutable(ShipId))
    {
        Ship->HullTier += 1;
        std::cout << "[ShipProgression] Hull tier upgraded for " << ShipId << " -> " << Ship->HullTier << "\n";
    }
}

void ShipProgressionSystem::UpgradeUtilityTier(const std::string& ShipId)
{
    if (auto* Ship = FindShipMutable(ShipId))
    {
        Ship->UtilityTier += 1;
        std::cout << "[ShipProgression] Utility tier upgraded for " << ShipId << " -> " << Ship->UtilityTier << "\n";
    }
}

void ShipProgressionSystem::UpgradeCargoTier(const std::string& ShipId)
{
    if (auto* Ship = FindShipMutable(ShipId))
    {
        Ship->CargoTier += 1;
        std::cout << "[ShipProgression] Cargo tier upgraded for " << ShipId << " -> " << Ship->CargoTier << "\n";
    }
}

void ShipProgressionSystem::Tick(float)
{
    std::cout << "[ShipProgression] Ships=" << Ships.size() << "\n";
}

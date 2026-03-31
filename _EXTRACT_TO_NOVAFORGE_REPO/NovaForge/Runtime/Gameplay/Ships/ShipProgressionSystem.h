#pragma once

#include "Gameplay/Ships/ShipProgressionTypes.h"
#include <string>
#include <vector>

class ShipProgressionSystem
{
public:
    bool Initialize();
    void RegisterShip(const ShipProgressionState& Ship);
    ShipProgressionState* FindShipMutable(const std::string& ShipId);
    const ShipProgressionState* FindShip(const std::string& ShipId) const;
    void UpgradeHullTier(const std::string& ShipId);
    void UpgradeUtilityTier(const std::string& ShipId);
    void UpgradeCargoTier(const std::string& ShipId);
    void Tick(float DeltaTime);

private:
    std::vector<ShipProgressionState> Ships;
};

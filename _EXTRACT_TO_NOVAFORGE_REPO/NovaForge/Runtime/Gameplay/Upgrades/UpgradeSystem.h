#pragma once

#include "Gameplay/Upgrades/UpgradeTypes.h"
#include <string>
#include <vector>

class UpgradeSystem
{
public:
    bool Initialize();
    void RegisterUpgrade(const UpgradeTier& Upgrade);
    const UpgradeTier* FindUpgrade(const std::string& UpgradeId) const;
    void Tick(float DeltaTime);

private:
    std::vector<UpgradeTier> Upgrades;
};

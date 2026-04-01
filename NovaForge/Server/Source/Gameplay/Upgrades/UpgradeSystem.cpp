#include "Gameplay/Upgrades/UpgradeSystem.h"
#include <iostream>

bool UpgradeSystem::Initialize()
{
    std::cout << "[Upgrades] Initialize\n";
    return true;
}

void UpgradeSystem::RegisterUpgrade(const UpgradeTier& Upgrade)
{
    Upgrades.push_back(Upgrade);
    std::cout << "[Upgrades] Registered " << Upgrade.UpgradeId << "\n";
}

const UpgradeTier* UpgradeSystem::FindUpgrade(const std::string& UpgradeId) const
{
    for (const auto& Upgrade : Upgrades)
    {
        if (Upgrade.UpgradeId == UpgradeId)
        {
            return &Upgrade;
        }
    }

    return nullptr;
}

void UpgradeSystem::Tick(float)
{
    std::cout << "[Upgrades] Count=" << Upgrades.size() << "\n";
}

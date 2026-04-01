#pragma once

#include <string>
#include <vector>

struct UpgradeModifier
{
    std::string Stat;
    float Value = 0.0f;
};

struct UpgradeTier
{
    std::string UpgradeId;
    std::string Category;
    int Tier = 1;
    std::vector<UpgradeModifier> Modifiers;
};

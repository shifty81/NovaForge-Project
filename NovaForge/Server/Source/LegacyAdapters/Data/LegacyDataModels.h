#pragma once

#include <string>
#include <vector>

struct LegacyItemRecord
{
    std::string Name;
    std::string Category;
    int Value = 0;
    int StackSize = 1;
};

struct LegacyRecipeInput
{
    std::string ItemName;
    int Count = 0;
};

struct LegacyRecipeRecord
{
    std::string Name;
    std::string Machine;
    int TimeSeconds = 0;
    std::vector<LegacyRecipeInput> Inputs;
};

struct LegacyMissionRecord
{
    std::string Title;
    std::string Type;
    int CreditReward = 0;
};

struct LegacyFactionRecord
{
    std::string Name;
    std::string Alignment;
};

struct LegacyModuleRecord
{
    std::string Name;
    std::string Category;
    int SizeX = 1;
    int SizeY = 1;
    int SizeZ = 1;
};

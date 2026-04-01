#pragma once

#include <string>
#include <vector>

struct MRItemDefinition
{
    std::string Id;
    std::string Name;
    std::string Category;
    int Value = 0;
    int StackSize = 1;
};

struct MRRecipeInput
{
    std::string ItemId;
    int Count = 0;
};

struct MRRecipeDefinition
{
    std::string Id;
    std::string Name;
    std::string Machine;
    int TimeSeconds = 0;
    std::vector<MRRecipeInput> Inputs;
};

struct MRMissionDefinition
{
    std::string Id;
    std::string Title;
    std::string Type;
    int CreditReward = 0;
};

struct MRFactionDefinition
{
    std::string Id;
    std::string Name;
    std::string Alignment;
};

struct MRModuleDefinition
{
    std::string Id;
    std::string Name;
    std::string Category;
    int SizeX = 1;
    int SizeY = 1;
    int SizeZ = 1;
};

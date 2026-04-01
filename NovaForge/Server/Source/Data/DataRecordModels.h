#pragma once

#include <string>
#include <vector>

struct ItemDefinition
{
    std::string Id;
    std::string Name;
    std::string Category;
    int StackSize = 1;
    int Value = 0;
};

struct RecipeIO
{
    std::string ItemId;
    int Count = 0;
};

struct RecipeDefinition
{
    std::string Id;
    std::string Name;
    std::string Machine;
    int TimeSeconds = 0;
    std::vector<RecipeIO> Inputs;
    std::vector<RecipeIO> Outputs;
};

struct MissionDefinition
{
    std::string Id;
    std::string Title;
    std::string Type;
    std::string GiverFaction;
    int CreditReward = 0;
};

struct FactionDefinition
{
    std::string Id;
    std::string Name;
    std::string Alignment;
};

struct LootEntry
{
    std::string ItemId;
    int Weight = 0;
    int Min = 0;
    int Max = 0;
};

struct LootTableDefinition
{
    std::string Id;
    int Rolls = 0;
    std::vector<LootEntry> Entries;
};

struct PlayerSeedItem
{
    std::string ItemId;
    int Count = 0;
};

struct PlayerDefinition
{
    std::string Id;
    std::string DisplayName;
    std::vector<PlayerSeedItem> StarterItems;
    std::vector<std::string> StarterMissions;
};

#pragma once

#include "Modules/ModuleRegistry.h"
#include <string>
#include <vector>

struct RecipeIngredient
{
    std::string ItemId;
    int         Count = 0;
};

struct RecipeDefinition
{
    std::string                     Id;
    std::vector<RecipeIngredient>   Inputs;
    std::vector<RecipeIngredient>   Outputs;
};

struct PlayerDefinition
{
    std::string Id;
    std::string DisplayName;
    std::vector<RecipeIngredient> StarterItems;
    std::vector<std::string>      StarterMissions;
};

struct ItemDefinition
{
    std::string Id;
    std::string Name;
    std::string Category;
    int         MaxStack = 99;
};

struct MissionDefinition
{
    std::string Id;
    std::string Title;
    std::string Description;
    int         ObjectiveCount = 1;
};

class DataRegistry
{
public:
    bool Initialize(const std::string& DataRoot);
    void Shutdown();

    const std::vector<ModuleDefinition>& GetLoadedModuleDefinitions() const;
    const RecipeDefinition*  FindRecipeDefinition(const std::string& RecipeId) const;
    const PlayerDefinition*  FindPlayerDefinition(const std::string& PlayerId) const;
    const ItemDefinition*    FindItemDefinition(const std::string& ItemId) const;
    const MissionDefinition* FindMissionDefinition(const std::string& MissionId) const;

private:
    bool LoadModuleDefinitions(const std::string& ModuleDirectory);

    std::vector<ModuleDefinition>  ModuleDefinitions;
    std::vector<RecipeDefinition>  RecipeDefinitions;
    std::vector<PlayerDefinition>  PlayerDefinitions;
    std::vector<ItemDefinition>    ItemDefinitions;
    std::vector<MissionDefinition> MissionDefinitions;
};

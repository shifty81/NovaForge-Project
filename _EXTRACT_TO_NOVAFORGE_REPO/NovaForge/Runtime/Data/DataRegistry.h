#pragma once

#include "Data/DataRecordModels.h"
#include "Modules/ModuleRegistry.h"
#include <string>
#include <unordered_map>
#include <vector>

class DataRegistry
{
public:
    bool Initialize(const std::string& DataRoot);
    void Shutdown();

    const std::vector<ModuleDefinition>& GetLoadedModuleDefinitions() const;
    const std::vector<ItemDefinition>& GetLoadedItemDefinitions() const;
    const std::vector<RecipeDefinition>& GetLoadedRecipeDefinitions() const;
    const std::vector<MissionDefinition>& GetLoadedMissionDefinitions() const;
    const std::vector<FactionDefinition>& GetLoadedFactionDefinitions() const;
    const std::vector<LootTableDefinition>& GetLoadedLootDefinitions() const;

    const ItemDefinition* FindItemDefinition(const std::string& Id) const;
    const RecipeDefinition* FindRecipeDefinition(const std::string& Id) const;
    const MissionDefinition* FindMissionDefinition(const std::string& Id) const;
    const FactionDefinition* FindFactionDefinition(const std::string& Id) const;
    const LootTableDefinition* FindLootDefinition(const std::string& Id) const;

private:
    bool LoadModuleDefinitions(const std::string& ModuleDirectory);
    bool LoadItemDefinitions(const std::string& ItemDirectory);
    bool LoadRecipeDefinitions(const std::string& RecipeDirectory);
    bool LoadMissionDefinitions(const std::string& MissionDirectory);
    bool LoadFactionDefinitions(const std::string& FactionDirectory);
    bool LoadLootDefinitions(const std::string& LootDirectory);
    void LogSummary() const;

    std::vector<ModuleDefinition> ModuleDefinitions;
    std::vector<ItemDefinition> ItemDefinitions;
    std::vector<RecipeDefinition> RecipeDefinitions;
    std::vector<MissionDefinition> MissionDefinitions;
    std::vector<FactionDefinition> FactionDefinitions;
    std::vector<LootTableDefinition> LootDefinitions;

    std::unordered_map<std::string, std::size_t> ItemIndexById;
    std::unordered_map<std::string, std::size_t> RecipeIndexById;
    std::unordered_map<std::string, std::size_t> MissionIndexById;
    std::unordered_map<std::string, std::size_t> FactionIndexById;
    std::unordered_map<std::string, std::size_t> LootIndexById;
};

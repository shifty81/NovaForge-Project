#include "Data/DataRegistry.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
std::string ReadTextFile(const std::filesystem::path& Path)
{
    std::ifstream Input(Path);
    std::ostringstream Buffer;
    Buffer << Input.rdbuf();
    return Buffer.str();
}

std::string ExtractString(const std::string& Text, const std::string& Key)
{
    const std::string Token = "\"" + Key + "\"";
    std::size_t KeyPos = Text.find(Token);
    if (KeyPos == std::string::npos) return "";
    std::size_t Colon = Text.find(':', KeyPos);
    std::size_t FirstQuote = Text.find('"', Colon + 1);
    std::size_t SecondQuote = Text.find('"', FirstQuote + 1);
    if (Colon == std::string::npos || FirstQuote == std::string::npos || SecondQuote == std::string::npos) return "";
    return Text.substr(FirstQuote + 1, SecondQuote - FirstQuote - 1);
}

int ExtractInt(const std::string& Text, const std::string& Key)
{
    const std::string Token = "\"" + Key + "\"";
    std::size_t KeyPos = Text.find(Token);
    if (KeyPos == std::string::npos) return 0;
    std::size_t ColonPos = Text.find(':', KeyPos);
    if (ColonPos == std::string::npos) return 0;
    std::size_t Start = Text.find_first_of("-0123456789", ColonPos + 1);
    if (Start == std::string::npos) return 0;
    std::size_t End = Text.find_first_not_of("-0123456789", Start);
    return std::stoi(Text.substr(Start, End - Start));
}

std::vector<std::string> ExtractAllStringsForKey(const std::string& Text, const std::string& Key)
{
    std::vector<std::string> Results;
    const std::string Token = "\"" + Key + "\"";
    std::size_t SearchPos = 0;

    while (true)
    {
        std::size_t KeyPos = Text.find(Token, SearchPos);
        if (KeyPos == std::string::npos) break;
        std::size_t Colon = Text.find(':', KeyPos);
        std::size_t FirstQuote = Text.find('"', Colon + 1);
        std::size_t SecondQuote = Text.find('"', FirstQuote + 1);
        if (Colon == std::string::npos || FirstQuote == std::string::npos || SecondQuote == std::string::npos) break;
        Results.push_back(Text.substr(FirstQuote + 1, SecondQuote - FirstQuote - 1));
        SearchPos = SecondQuote + 1;
    }

    return Results;
}

std::vector<int> ExtractAllIntsForKey(const std::string& Text, const std::string& Key)
{
    std::vector<int> Results;
    const std::string Token = "\"" + Key + "\"";
    std::size_t SearchPos = 0;

    while (true)
    {
        std::size_t KeyPos = Text.find(Token, SearchPos);
        if (KeyPos == std::string::npos) break;
        std::size_t ColonPos = Text.find(':', KeyPos);
        if (ColonPos == std::string::npos) break;
        std::size_t Start = Text.find_first_of("-0123456789", ColonPos + 1);
        if (Start == std::string::npos) break;
        std::size_t End = Text.find_first_not_of("-0123456789", Start);
        Results.push_back(std::stoi(Text.substr(Start, End - Start)));
        SearchPos = End;
    }

    return Results;
}

template <typename T>
bool LoadFilesInDirectory(const std::string& Directory, const std::string& ExpectedExtension, T&& Loader)
{
    namespace fs = std::filesystem;
    if (!fs::exists(Directory))
    {
        std::cerr << "[DataRegistry] Missing directory: " << Directory << "\n";
        return false;
    }

    bool bLoadedAny = false;
    for (const auto& Entry : fs::directory_iterator(Directory))
    {
        if (!Entry.is_regular_file())
        {
            continue;
        }

        const std::string Filename = Entry.path().filename().string();
        if (Filename.size() < ExpectedExtension.size() ||
            Filename.substr(Filename.size() - ExpectedExtension.size()) != ExpectedExtension)
        {
            continue;
        }

        Loader(Entry.path());
        bLoadedAny = true;
    }

    return bLoadedAny;
}
}

bool DataRegistry::Initialize(const std::string& DataRoot)
{
    std::cout << "[DataRegistry] Initialize Root=" << DataRoot << "\n";

    const bool bModules = LoadModuleDefinitions(DataRoot + "/Definitions/Modules");
    const bool bItems = LoadItemDefinitions(DataRoot + "/Definitions/Items");
    const bool bRecipes = LoadRecipeDefinitions(DataRoot + "/Definitions/Recipes");
    const bool bMissions = LoadMissionDefinitions(DataRoot + "/Definitions/Missions");
    const bool bFactions = LoadFactionDefinitions(DataRoot + "/Definitions/Factions");
    const bool bLoot = LoadLootDefinitions(DataRoot + "/Definitions/Loot");

    LogSummary();
    return bModules || bItems || bRecipes || bMissions || bFactions || bLoot;
}

void DataRegistry::Shutdown()
{
    std::cout << "[DataRegistry] Shutdown\n";
    ModuleDefinitions.clear();
    ItemDefinitions.clear();
    RecipeDefinitions.clear();
    MissionDefinitions.clear();
    FactionDefinitions.clear();
    LootDefinitions.clear();

    ItemIndexById.clear();
    RecipeIndexById.clear();
    MissionIndexById.clear();
    FactionIndexById.clear();
    LootIndexById.clear();
}

const std::vector<ModuleDefinition>& DataRegistry::GetLoadedModuleDefinitions() const { return ModuleDefinitions; }
const std::vector<ItemDefinition>& DataRegistry::GetLoadedItemDefinitions() const { return ItemDefinitions; }
const std::vector<RecipeDefinition>& DataRegistry::GetLoadedRecipeDefinitions() const { return RecipeDefinitions; }
const std::vector<MissionDefinition>& DataRegistry::GetLoadedMissionDefinitions() const { return MissionDefinitions; }
const std::vector<FactionDefinition>& DataRegistry::GetLoadedFactionDefinitions() const { return FactionDefinitions; }
const std::vector<LootTableDefinition>& DataRegistry::GetLoadedLootDefinitions() const { return LootDefinitions; }

const ItemDefinition* DataRegistry::FindItemDefinition(const std::string& Id) const
{
    auto It = ItemIndexById.find(Id);
    return It != ItemIndexById.end() ? &ItemDefinitions[It->second] : nullptr;
}

const RecipeDefinition* DataRegistry::FindRecipeDefinition(const std::string& Id) const
{
    auto It = RecipeIndexById.find(Id);
    return It != RecipeIndexById.end() ? &RecipeDefinitions[It->second] : nullptr;
}

const MissionDefinition* DataRegistry::FindMissionDefinition(const std::string& Id) const
{
    auto It = MissionIndexById.find(Id);
    return It != MissionIndexById.end() ? &MissionDefinitions[It->second] : nullptr;
}

const FactionDefinition* DataRegistry::FindFactionDefinition(const std::string& Id) const
{
    auto It = FactionIndexById.find(Id);
    return It != FactionIndexById.end() ? &FactionDefinitions[It->second] : nullptr;
}

const LootTableDefinition* DataRegistry::FindLootDefinition(const std::string& Id) const
{
    auto It = LootIndexById.find(Id);
    return It != LootIndexById.end() ? &LootDefinitions[It->second] : nullptr;
}

bool DataRegistry::LoadModuleDefinitions(const std::string& ModuleDirectory)
{
    ModuleDefinitions.clear();

    return LoadFilesInDirectory(ModuleDirectory, ".json", [&](const std::filesystem::path& Path)
    {
        const std::string Text = ReadTextFile(Path);
        ModuleDefinition Def;
        Def.Id = ExtractString(Text, "id");
        Def.Name = ExtractString(Text, "name");
        Def.Category = ExtractString(Text, "category");
        Def.SizeX = ExtractInt(Text, "x");
        Def.SizeY = ExtractInt(Text, "y");
        Def.SizeZ = ExtractInt(Text, "z");

        if (!Def.Id.empty())
        {
            ModuleDefinitions.push_back(Def);
            std::cout << "[DataRegistry] Loaded module " << Def.Id << "\n";
        }
    });
}

bool DataRegistry::LoadItemDefinitions(const std::string& ItemDirectory)
{
    ItemDefinitions.clear();
    ItemIndexById.clear();

    return LoadFilesInDirectory(ItemDirectory, ".item.json", [&](const std::filesystem::path& Path)
    {
        const std::string Text = ReadTextFile(Path);
        ItemDefinition Def;
        Def.Id = ExtractString(Text, "id");
        Def.Name = ExtractString(Text, "name");
        Def.Category = ExtractString(Text, "category");
        Def.StackSize = ExtractInt(Text, "stack_size");
        Def.Value = ExtractInt(Text, "value");

        if (!Def.Id.empty())
        {
            ItemIndexById[Def.Id] = ItemDefinitions.size();
            ItemDefinitions.push_back(Def);
            std::cout << "[DataRegistry] Loaded item " << Def.Id << "\n";
        }
    });
}

bool DataRegistry::LoadRecipeDefinitions(const std::string& RecipeDirectory)
{
    RecipeDefinitions.clear();
    RecipeIndexById.clear();

    return LoadFilesInDirectory(RecipeDirectory, ".recipe.json", [&](const std::filesystem::path& Path)
    {
        const std::string Text = ReadTextFile(Path);
        RecipeDefinition Def;
        Def.Id = ExtractString(Text, "id");
        Def.Name = ExtractString(Text, "name");
        Def.Machine = ExtractString(Text, "machine");
        Def.TimeSeconds = ExtractInt(Text, "time_seconds");

        const auto ItemIds = ExtractAllStringsForKey(Text, "item_id");
        const auto Counts = ExtractAllIntsForKey(Text, "count");

        const std::size_t Half = ItemIds.size() > 0 ? (Counts.size() >= ItemIds.size() ? ItemIds.size() / 2 : 0) : 0;
        for (std::size_t i = 0; i < ItemIds.size(); ++i)
        {
            RecipeIO IO;
            IO.ItemId = ItemIds[i];
            IO.Count = i < Counts.size() ? Counts[i] : 0;

            if (i < Half)
            {
                Def.Inputs.push_back(IO);
            }
            else
            {
                Def.Outputs.push_back(IO);
            }
        }

        if (!Def.Id.empty())
        {
            RecipeIndexById[Def.Id] = RecipeDefinitions.size();
            RecipeDefinitions.push_back(Def);
            std::cout << "[DataRegistry] Loaded recipe " << Def.Id << "\n";
        }
    });
}

bool DataRegistry::LoadMissionDefinitions(const std::string& MissionDirectory)
{
    MissionDefinitions.clear();
    MissionIndexById.clear();

    return LoadFilesInDirectory(MissionDirectory, ".mission.json", [&](const std::filesystem::path& Path)
    {
        const std::string Text = ReadTextFile(Path);
        MissionDefinition Def;
        Def.Id = ExtractString(Text, "id");
        Def.Title = ExtractString(Text, "title");
        Def.Type = ExtractString(Text, "type");
        Def.GiverFaction = ExtractString(Text, "giver_faction");
        Def.CreditReward = ExtractInt(Text, "credits");

        if (!Def.Id.empty())
        {
            MissionIndexById[Def.Id] = MissionDefinitions.size();
            MissionDefinitions.push_back(Def);
            std::cout << "[DataRegistry] Loaded mission " << Def.Id << "\n";
        }
    });
}

bool DataRegistry::LoadFactionDefinitions(const std::string& FactionDirectory)
{
    FactionDefinitions.clear();
    FactionIndexById.clear();

    return LoadFilesInDirectory(FactionDirectory, ".faction.json", [&](const std::filesystem::path& Path)
    {
        const std::string Text = ReadTextFile(Path);
        FactionDefinition Def;
        Def.Id = ExtractString(Text, "id");
        Def.Name = ExtractString(Text, "name");
        Def.Alignment = ExtractString(Text, "alignment");

        if (!Def.Id.empty())
        {
            FactionIndexById[Def.Id] = FactionDefinitions.size();
            FactionDefinitions.push_back(Def);
            std::cout << "[DataRegistry] Loaded faction " << Def.Id << "\n";
        }
    });
}

bool DataRegistry::LoadLootDefinitions(const std::string& LootDirectory)
{
    LootDefinitions.clear();
    LootIndexById.clear();

    return LoadFilesInDirectory(LootDirectory, ".loot.json", [&](const std::filesystem::path& Path)
    {
        const std::string Text = ReadTextFile(Path);
        LootTableDefinition Def;
        Def.Id = ExtractString(Text, "id");
        Def.Rolls = ExtractInt(Text, "rolls");

        const auto ItemIds = ExtractAllStringsForKey(Text, "item_id");
        const auto Weights = ExtractAllIntsForKey(Text, "weight");
        const auto Mins = ExtractAllIntsForKey(Text, "min");
        const auto Maxs = ExtractAllIntsForKey(Text, "max");

        for (std::size_t i = 0; i < ItemIds.size(); ++i)
        {
            LootEntry Entry;
            Entry.ItemId = ItemIds[i];
            Entry.Weight = i < Weights.size() ? Weights[i] : 0;
            Entry.Min = i < Mins.size() ? Mins[i] : 0;
            Entry.Max = i < Maxs.size() ? Maxs[i] : 0;
            Def.Entries.push_back(Entry);
        }

        if (!Def.Id.empty())
        {
            LootIndexById[Def.Id] = LootDefinitions.size();
            LootDefinitions.push_back(Def);
            std::cout << "[DataRegistry] Loaded loot " << Def.Id << "\n";
        }
    });
}

void DataRegistry::LogSummary() const
{
    std::cout << "[DataRegistry] Summary"
              << " Modules=" << ModuleDefinitions.size()
              << " Items=" << ItemDefinitions.size()
              << " Recipes=" << RecipeDefinitions.size()
              << " Missions=" << MissionDefinitions.size()
              << " Factions=" << FactionDefinitions.size()
              << " Loot=" << LootDefinitions.size()
              << "\n";
}

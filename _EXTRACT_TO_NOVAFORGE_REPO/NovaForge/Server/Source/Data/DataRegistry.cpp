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
    std::size_t FirstQuote = Text.find('"', Text.find(':', KeyPos) + 1);
    std::size_t SecondQuote = Text.find('"', FirstQuote + 1);
    if (FirstQuote == std::string::npos || SecondQuote == std::string::npos) return "";
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
    std::size_t End = Text.find_first_not_of("-0123456789", Start);
    if (Start == std::string::npos) return 0;
    return std::stoi(Text.substr(Start, End - Start));
}
}

bool DataRegistry::Initialize(const std::string& DataRoot)
{
    std::cout << "[DataRegistry] Initialize Root=" << DataRoot << "\n";
    return LoadModuleDefinitions(DataRoot + "/Definitions/Modules");
}

void DataRegistry::Shutdown()
{
    std::cout << "[DataRegistry] Shutdown\n";
    ModuleDefinitions.clear();
}

const std::vector<ModuleDefinition>& DataRegistry::GetLoadedModuleDefinitions() const
{
    return ModuleDefinitions;
}

const RecipeDefinition* DataRegistry::FindRecipeDefinition(const std::string& RecipeId) const
{
    for (const auto& Recipe : RecipeDefinitions)
    {
        if (Recipe.Id == RecipeId) return &Recipe;
    }
    return nullptr;
}

const PlayerDefinition* DataRegistry::FindPlayerDefinition(const std::string& PlayerId) const
{
    for (const auto& Player : PlayerDefinitions)
    {
        if (Player.Id == PlayerId) return &Player;
    }
    return nullptr;
}

const ItemDefinition* DataRegistry::FindItemDefinition(const std::string& ItemId) const
{
    for (const auto& Item : ItemDefinitions)
    {
        if (Item.Id == ItemId) return &Item;
    }
    return nullptr;
}

const MissionDefinition* DataRegistry::FindMissionDefinition(const std::string& MissionId) const
{
    for (const auto& Mission : MissionDefinitions)
    {
        if (Mission.Id == MissionId) return &Mission;
    }
    return nullptr;
}

bool DataRegistry::LoadModuleDefinitions(const std::string& ModuleDirectory)
{
    namespace fs = std::filesystem;
    ModuleDefinitions.clear();

    if (!fs::exists(ModuleDirectory))
    {
        std::cerr << "[DataRegistry] Missing module directory: " << ModuleDirectory << "\n";
        return false;
    }

    for (const auto& Entry : fs::directory_iterator(ModuleDirectory))
    {
        if (!Entry.is_regular_file() || Entry.path().extension() != ".json")
        {
            continue;
        }

        const std::string Text = ReadTextFile(Entry.path());
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
    }

    return !ModuleDefinitions.empty();
}

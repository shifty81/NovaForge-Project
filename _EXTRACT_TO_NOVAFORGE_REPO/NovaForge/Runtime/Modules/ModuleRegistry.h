#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct ModuleDefinition
{
    std::string Id;
    std::string Name;
    std::string Category;
    int SizeX = 1;
    int SizeY = 1;
    int SizeZ = 1;
};

class ModuleRegistry
{
public:
    bool Initialize();
    void RegisterDefinition(const ModuleDefinition& Definition);
    const ModuleDefinition* FindDefinition(const std::string& ModuleId) const;
    std::size_t GetDefinitionCount() const;
    void Shutdown();

private:
    std::unordered_map<std::string, ModuleDefinition> Definitions;
};

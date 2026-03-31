#include "Modules/ModuleRegistry.h"
#include <iostream>

bool ModuleRegistry::Initialize()
{
    std::cout << "[ModuleRegistry] Initialize\n";
    return true;
}

void ModuleRegistry::RegisterDefinition(const ModuleDefinition& Definition)
{
    Definitions[Definition.Id] = Definition;
    std::cout << "[ModuleRegistry] Registered Definition " << Definition.Id << "\n";
}

const ModuleDefinition* ModuleRegistry::FindDefinition(const std::string& ModuleId) const
{
    auto It = Definitions.find(ModuleId);
    return It != Definitions.end() ? &It->second : nullptr;
}

std::size_t ModuleRegistry::GetDefinitionCount() const
{
    return Definitions.size();
}

void ModuleRegistry::Shutdown()
{
    std::cout << "[ModuleRegistry] Shutdown\n";
    Definitions.clear();
}

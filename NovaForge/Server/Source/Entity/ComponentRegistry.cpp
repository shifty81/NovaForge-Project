#include "Entity/ComponentRegistry.h"
#include <iostream>

bool ComponentRegistry::Initialize()
{
    std::cout << "[Components] Initialize\n";
    return true;
}

void ComponentRegistry::AddNameComponent(EntityId Entity, const std::string& Name)
{
    NameComponents[Entity.Value] = NameComponent{Name};
    std::cout << "[Components] NameComponent added to Entity " << Entity.ToString()
              << " Name=" << Name << "\n";
}

const NameComponent* ComponentRegistry::FindNameComponent(EntityId Entity) const
{
    auto It = NameComponents.find(Entity.Value);
    return It != NameComponents.end() ? &It->second : nullptr;
}

void ComponentRegistry::Shutdown()
{
    std::cout << "[Components] Shutdown\n";
    NameComponents.clear();
}

#include "Entity/EntityRegistry.h"
#include <iostream>

bool EntityRegistry::Initialize()
{
    std::cout << "[Entities] Initialize\n";
    return true;
}

EntityId EntityRegistry::CreateEntity(const std::string& Name)
{
    EntityId Id{NextId++};
    Records.emplace(Id.Value, EntityRecord{Id, Name});
    std::cout << "[Entities] Created Entity " << Name << " Id=" << Id.ToString() << "\n";
    return Id;
}

const EntityRecord* EntityRegistry::FindEntity(EntityId Id) const
{
    auto It = Records.find(Id.Value);
    return It != Records.end() ? &It->second : nullptr;
}

std::size_t EntityRegistry::GetEntityCount() const
{
    return Records.size();
}

void EntityRegistry::Shutdown()
{
    std::cout << "[Entities] Shutdown\n";
    Records.clear();
}

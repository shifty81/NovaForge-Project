#pragma once

#include "Entity/EntityTypes.h"
#include <string>
#include <unordered_map>

struct NameComponent
{
    std::string DisplayName;
};

class ComponentRegistry
{
public:
    bool Initialize();
    void AddNameComponent(EntityId Entity, const std::string& Name);
    const NameComponent* FindNameComponent(EntityId Entity) const;
    void Shutdown();

private:
    std::unordered_map<std::uint64_t, NameComponent> NameComponents;
};

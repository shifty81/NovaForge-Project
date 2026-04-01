#pragma once

#include "Entity/EntityTypes.h"
#include <string>
#include <unordered_map>

struct EntityRecord
{
    EntityId Id;
    std::string Name;
};

class EntityRegistry
{
public:
    bool Initialize();
    EntityId CreateEntity(const std::string& Name);
    const EntityRecord* FindEntity(EntityId Id) const;
    std::size_t GetEntityCount() const;
    void Shutdown();

private:
    std::uint64_t NextId = 1;
    std::unordered_map<std::uint64_t, EntityRecord> Records;
};

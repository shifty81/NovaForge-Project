#pragma once

#include "Entity/EntityTypes.h"
#include <memory>
#include <string>
#include <vector>

class DataRegistry;
class StructureRegistry;
class EntityRegistry;
class ModuleRegistry;

struct ModuleInstance
{
    std::string InstanceId;
    std::string DefinitionId;
    std::string StructureId;
    EntityId RootEntity;
};

class ModuleSubsystem
{
public:
    bool Initialize(DataRegistry& Data, StructureRegistry& Structures, EntityRegistry& Entities);
    void Tick(float DeltaTime);
    void Shutdown();

    std::size_t GetModuleCount() const;

private:
    bool SpawnTestModule();

    DataRegistry* DataRef = nullptr;
    StructureRegistry* StructureRef = nullptr;
    EntityRegistry* EntityRef = nullptr;
    std::unique_ptr<ModuleRegistry> Registry;
    std::vector<ModuleInstance> Instances;
};

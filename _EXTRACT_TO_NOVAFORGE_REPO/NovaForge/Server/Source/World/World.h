#pragma once

#include "Entity/ComponentRegistry.h"
#include "Entity/EntityRegistry.h"
#include "Modules/ModuleSubsystem.h"
#include "Voxel/StructureRegistry.h"
#include "Voxel/VoxelSubsystem.h"
#include "World/SystemScheduler.h"

#include <memory>
#include <string>

class DataRegistry;

class World
{
public:
    ~World();
    bool Initialize(DataRegistry& InDataRegistry);
    void Tick(float DeltaTime);
    void Shutdown();

    EntityRegistry& GetEntityRegistry();
    ComponentRegistry& GetComponentRegistry();
    StructureRegistry& GetStructureRegistry();
    VoxelSubsystem& GetVoxelSubsystem();
    const VoxelSubsystem& GetVoxelSubsystem() const;
    ModuleSubsystem& GetModuleSubsystem();
    const ModuleSubsystem& GetModuleSubsystem() const;

    const std::string& GetWorldName() const;

private:
    DataRegistry* Data = nullptr;
    std::unique_ptr<EntityRegistry> Entities;
    std::unique_ptr<ComponentRegistry> Components;
    std::unique_ptr<StructureRegistry> Structures;
    std::unique_ptr<VoxelSubsystem> Voxels;
    std::unique_ptr<ModuleSubsystem> Modules;
    std::unique_ptr<SystemScheduler> Scheduler;
    std::string WorldName = "NovaForgeDevWorld";
};

#pragma once

#include <memory>
#include <string>

class DataRegistry;
class EntityRegistry;
class ComponentRegistry;
class StructureRegistry;
class VoxelSubsystem;
class ModuleSubsystem;
class SystemScheduler;

class World
{
public:
    bool Initialize(DataRegistry& InDataRegistry);
    void Tick(float DeltaTime);
    void Shutdown();

    EntityRegistry& GetEntityRegistry();
    ComponentRegistry& GetComponentRegistry();
    StructureRegistry& GetStructureRegistry();
    VoxelSubsystem& GetVoxelSubsystem();
    ModuleSubsystem& GetModuleSubsystem();

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

#include "World/World.h"
#include "Data/DataRegistry.h"
#include "Entity/ComponentRegistry.h"
#include "Entity/EntityRegistry.h"
#include "Modules/ModuleSubsystem.h"
#include "Voxel/StructureRegistry.h"
#include "Voxel/VoxelSubsystem.h"
#include "World/SystemScheduler.h"
#include <iostream>

bool World::Initialize(DataRegistry& InDataRegistry)
{
    std::cout << "[World] Initialize: " << WorldName << "\n";
    Data = &InDataRegistry;

    Entities = std::make_unique<EntityRegistry>();
    Components = std::make_unique<ComponentRegistry>();
    Structures = std::make_unique<StructureRegistry>();
    Voxels = std::make_unique<VoxelSubsystem>();
    Modules = std::make_unique<ModuleSubsystem>();
    Scheduler = std::make_unique<SystemScheduler>();

    if (!Entities->Initialize())
    {
        return false;
    }

    if (!Components->Initialize())
    {
        return false;
    }

    if (!Structures->Initialize())
    {
        return false;
    }

    if (!Voxels->Initialize(*Structures))
    {
        return false;
    }

    if (!Modules->Initialize(InDataRegistry, *Structures, *Entities))
    {
        return false;
    }

    Scheduler->Initialize();
    return true;
}

void World::Tick(float DeltaTime)
{
    Scheduler->Tick(DeltaTime, *this);
}

void World::Shutdown()
{
    std::cout << "[World] Shutdown\n";

    if (Scheduler)
    {
        Scheduler->Shutdown();
        Scheduler.reset();
    }

    if (Modules)
    {
        Modules->Shutdown();
        Modules.reset();
    }

    if (Voxels)
    {
        Voxels->Shutdown();
        Voxels.reset();
    }

    if (Structures)
    {
        Structures->Shutdown();
        Structures.reset();
    }

    if (Components)
    {
        Components->Shutdown();
        Components.reset();
    }

    if (Entities)
    {
        Entities->Shutdown();
        Entities.reset();
    }

    Data = nullptr;
}

EntityRegistry& World::GetEntityRegistry() { return *Entities; }
ComponentRegistry& World::GetComponentRegistry() { return *Components; }
StructureRegistry& World::GetStructureRegistry() { return *Structures; }
VoxelSubsystem& World::GetVoxelSubsystem() { return *Voxels; }
ModuleSubsystem& World::GetModuleSubsystem() { return *Modules; }
const std::string& World::GetWorldName() const { return WorldName; }

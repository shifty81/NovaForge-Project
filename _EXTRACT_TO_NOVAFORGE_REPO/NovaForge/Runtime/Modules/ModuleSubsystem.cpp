#include "Modules/ModuleSubsystem.h"
#include "Data/DataRegistry.h"
#include "Entity/EntityRegistry.h"
#include "Modules/ModuleRegistry.h"
#include "Voxel/StructureRegistry.h"
#include <iostream>

bool ModuleSubsystem::Initialize(DataRegistry& Data, StructureRegistry& Structures, EntityRegistry& Entities)
{
    std::cout << "[Modules] Initialize\n";
    DataRef = &Data;
    StructureRef = &Structures;
    EntityRef = &Entities;

    Registry = std::make_unique<ModuleRegistry>();
    Registry->Initialize();

    for (const auto& Definition : Data.GetLoadedModuleDefinitions())
    {
        Registry->RegisterDefinition(Definition);
    }

    return SpawnTestModule();
}

void ModuleSubsystem::Tick(float)
{
    std::cout << "[Modules] Definitions=" << Registry->GetDefinitionCount()
              << " Instances=" << Instances.size() << "\n";
}

void ModuleSubsystem::Shutdown()
{
    std::cout << "[Modules] Shutdown\n";
    Instances.clear();

    if (Registry)
    {
        Registry->Shutdown();
        Registry.reset();
    }

    DataRef = nullptr;
    StructureRef = nullptr;
    EntityRef = nullptr;
}

std::size_t ModuleSubsystem::GetModuleCount() const
{
    return Instances.size();
}

bool ModuleSubsystem::SpawnTestModule()
{
    if (!Registry->FindDefinition("reactor_mk1"))
    {
        std::cerr << "[Modules] Missing reactor_mk1 definition\n";
        return false;
    }

    if (!StructureRef->FindStructure("ship_dev_001"))
    {
        std::cerr << "[Modules] Missing ship_dev_001 structure\n";
        return false;
    }

    EntityId Entity = EntityRef->CreateEntity("Module_Reactor_MK1");
    Instances.push_back({"mod_0001", "reactor_mk1", "ship_dev_001", Entity});
    std::cout << "[Modules] Spawned Test Module mod_0001 on ship_dev_001\n";
    return true;
}

#include "World/SystemScheduler.h"
#include "Modules/ModuleSubsystem.h"
#include "Voxel/VoxelSubsystem.h"
#include "World/World.h"
#include <iostream>

bool SystemScheduler::Initialize()
{
    std::cout << "[Scheduler] Initialize\n";
    return true;
}

void SystemScheduler::Tick(float DeltaTime, World& InWorld)
{
    ++TickCounter;
    std::cout << "[Scheduler] TickCounter=" << TickCounter
              << " DeltaTime=" << DeltaTime << "\n";

    InWorld.GetVoxelSubsystem().Tick(DeltaTime);
    InWorld.GetModuleSubsystem().Tick(DeltaTime);
}

void SystemScheduler::Shutdown()
{
    std::cout << "[Scheduler] Shutdown\n";
}

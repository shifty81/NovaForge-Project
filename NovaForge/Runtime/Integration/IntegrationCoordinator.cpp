#include "Integration/IntegrationCoordinator.h"
#include <iostream>

bool IntegrationCoordinator::Initialize()
{
    Checkpoints = {
        {"WorldBoot", false},
        {"PlayerSpawn", false},
        {"ShipInteriorReady", false},
        {"EVAReady", false},
        {"SalvageReady", false},
        {"MiningReady", false},
        {"StationReady", false},
        {"ContractsReady", false},
        {"ProgressionReady", false}
    };
    std::cout << "[Integration] Initialize\n";
    return true;
}

void IntegrationCoordinator::BootstrapPlayableLoop()
{
    for (auto& Checkpoint : Checkpoints)
    {
        Checkpoint.bComplete = true;
        std::cout << "[Integration] " << Checkpoint.Name << " complete\n";
    }
}

void IntegrationCoordinator::Tick(float)
{
    int CompleteCount = 0;
    for (const auto& Checkpoint : Checkpoints)
    {
        if (Checkpoint.bComplete) ++CompleteCount;
    }

    std::cout << "[Integration] Checkpoints " << CompleteCount
              << "/" << Checkpoints.size() << "\n";
}

void IntegrationCoordinator::Shutdown()
{
    Checkpoints.clear();
    std::cout << "[Integration] Shutdown\n";
}

const std::vector<IntegrationCheckpoint>& IntegrationCoordinator::GetCheckpoints() const
{
    return Checkpoints;
}

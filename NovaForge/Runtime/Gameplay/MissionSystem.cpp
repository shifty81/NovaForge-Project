#include "Gameplay/MissionSystem.h"
#include "Data/DataRegistry.h"
#include <iostream>

bool MissionSystem::Initialize(DataRegistry& InDataRegistry)
{
    Data = &InDataRegistry;
    std::cout << "[MissionSystem] Initialize\n";
    return true;
}

bool MissionSystem::AcceptMission(const std::string& MissionId)
{
    if (!Data || !Data->FindMissionDefinition(MissionId))
    {
        return false;
    }

    for (const auto& Mission : Missions)
    {
        if (Mission.MissionId == MissionId)
        {
            return false;
        }
    }

    Missions.push_back({MissionId, true, false, 0});
    std::cout << "[MissionSystem] Accepted " << MissionId << "\n";
    return true;
}

bool MissionSystem::CompleteMission(const std::string& MissionId)
{
    for (auto& Mission : Missions)
    {
        if (Mission.MissionId == MissionId && Mission.bAccepted && !Mission.bCompleted)
        {
            Mission.bCompleted = true;
            std::cout << "[MissionSystem] Completed " << MissionId << "\n";
            return true;
        }
    }

    return false;
}

const std::vector<MissionProgress>& MissionSystem::GetMissionLog() const
{
    return Missions;
}

void MissionSystem::LogMissionState() const
{
    std::cout << "[MissionSystem] Missions=" << Missions.size() << "\n";
    for (const auto& Mission : Missions)
    {
        std::cout << "  - " << Mission.MissionId
                  << " Accepted=" << (Mission.bAccepted ? "true" : "false")
                  << " Completed=" << (Mission.bCompleted ? "true" : "false")
                  << "\n";
    }
}

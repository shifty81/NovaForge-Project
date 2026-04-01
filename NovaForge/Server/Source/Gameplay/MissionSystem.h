#pragma once

#include "Gameplay/GameplayTypes.h"
#include <string>
#include <vector>

class DataRegistry;

class MissionSystem
{
public:
    bool Initialize(DataRegistry& InDataRegistry);

    bool AcceptMission(const std::string& MissionId);
    bool CompleteMission(const std::string& MissionId);
    const std::vector<MissionProgress>& GetMissionLog() const;
    void LogMissionState() const;

private:
    DataRegistry* Data = nullptr;
    std::vector<MissionProgress> Missions;
};

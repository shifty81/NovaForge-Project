#pragma once

#include <string>

namespace Runtime::Session
{
struct SessionBootstrapConfig
{
std::string ScenarioId = "VerticalSlice_StarterDerelict";
std::string SaveSlotId;
bool bLoadFromSave = false;
bool bEnableDebugOverrides = false;
unsigned int DeterministicSeed = 0;
};

class SessionBootstrap
{
public:
    bool Boot(const SessionBootstrapConfig& config);
    void Shutdown();

private:
    bool LoadScenario(const SessionBootstrapConfig& config);
    bool RestoreSaveIfNeeded(const SessionBootstrapConfig& config);
    bool SpawnPlayerRig(const SessionBootstrapConfig& config);
    bool StartMissionContext(const SessionBootstrapConfig& config);
};
}

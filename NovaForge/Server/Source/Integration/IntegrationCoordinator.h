#pragma once

#include <string>
#include <vector>

struct IntegrationCheckpoint
{
    std::string Name;
    bool bComplete = false;
};

class IntegrationCoordinator
{
public:
    bool Initialize();
    void BootstrapPlayableLoop();
    void Tick(float DeltaTime);
    void Shutdown();

    const std::vector<IntegrationCheckpoint>& GetCheckpoints() const;

private:
    std::vector<IntegrationCheckpoint> Checkpoints;
};

#pragma once

#include <string>
#include <vector>

struct OverlayMetric
{
    std::string Name;
    std::string Value;
};

class DevOverlayState
{
public:
    bool Initialize();
    void Tick(float DeltaTime);
    void Shutdown();

    void SetMetric(const std::string& Name, const std::string& Value);

private:
    std::vector<OverlayMetric> Metrics;
};

#include "Debug/DevOverlayState.h"
#include <iostream>

bool DevOverlayState::Initialize()
{
    Metrics = {
        {"PlayerMode", "Interior"},
        {"CurrentSector", "sector_home"},
        {"ActiveContract", "contract_salvage_panels_01"},
        {"Season", "1"},
        {"TitanProgress", "0%"}
    };

    std::cout << "[DevOverlay] Initialize\n";
    return true;
}

void DevOverlayState::Tick(float)
{
    std::cout << "[DevOverlay] Metrics=" << Metrics.size() << "\n";
    for (const auto& Metric : Metrics)
    {
        std::cout << "  DBG> " << Metric.Name << ": " << Metric.Value << "\n";
    }
}

void DevOverlayState::Shutdown()
{
    Metrics.clear();
    std::cout << "[DevOverlay] Shutdown\n";
}

void DevOverlayState::SetMetric(const std::string& Name, const std::string& Value)
{
    for (auto& Metric : Metrics)
    {
        if (Metric.Name == Name)
        {
            Metric.Value = Value;
            return;
        }
    }
    Metrics.push_back({Name, Value});
}

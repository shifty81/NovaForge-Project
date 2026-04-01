#include "Gameplay/Stations/StationServiceSystem.h"
#include <iostream>

bool StationServiceSystem::Initialize()
{
    std::cout << "[Stations] Initialize\n";
    return true;
}

void StationServiceSystem::RegisterStation(const StationDefinitionRuntime& Station)
{
    Stations.push_back(Station);
    std::cout << "[Stations] Registered " << Station.StationId << "\n";
}

const StationDefinitionRuntime* StationServiceSystem::FindStation(const std::string& StationId) const
{
    for (const auto& Station : Stations)
    {
        if (Station.StationId == StationId)
        {
            return &Station;
        }
    }
    return nullptr;
}

bool StationServiceSystem::HasService(const std::string& StationId, const std::string& ServiceId) const
{
    const auto* Station = FindStation(StationId);
    if (!Station) return false;

    for (const auto& Service : Station->Services)
    {
        if (Service.ServiceId == ServiceId && Service.bAvailable)
        {
            return true;
        }
    }
    return false;
}

void StationServiceSystem::Tick(float)
{
    std::cout << "[Stations] Count=" << Stations.size() << "\n";
}

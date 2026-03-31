#pragma once

#include "Gameplay/Stations/StationTypes.h"
#include <string>
#include <vector>

class StationServiceSystem
{
public:
    bool Initialize();
    void RegisterStation(const StationDefinitionRuntime& Station);
    const StationDefinitionRuntime* FindStation(const std::string& StationId) const;
    bool HasService(const std::string& StationId, const std::string& ServiceId) const;
    void Tick(float DeltaTime);

private:
    std::vector<StationDefinitionRuntime> Stations;
};

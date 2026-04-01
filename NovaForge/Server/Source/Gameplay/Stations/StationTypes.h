#pragma once

#include <string>
#include <vector>

struct StationServiceFlag
{
    std::string ServiceId;
    bool bAvailable = false;
};

struct StationDefinitionRuntime
{
    std::string StationId;
    std::string Name;
    int Tier = 1;
    std::vector<StationServiceFlag> Services;
};

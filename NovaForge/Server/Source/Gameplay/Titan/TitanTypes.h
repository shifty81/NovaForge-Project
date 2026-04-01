#pragma once

#include <string>
#include <vector>

struct TitanRequirement
{
    std::string ResourceId;
    int RequiredAmount = 0;
    int CurrentAmount = 0;
};

struct TitanProjectState
{
    std::string TitanId;
    std::string DisplayName;
    bool bConstructionStarted = false;
    bool bConstructionComplete = false;
    std::vector<TitanRequirement> Requirements;
};

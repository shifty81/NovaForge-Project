#pragma once
#include <string>

struct SectorState
{
    std::string SectorId;
    std::string ControllingFaction;
    int ThreatLevel = 0;
};
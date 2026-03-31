#pragma once

#include <string>
#include <vector>

enum class EFleetAssignmentType
{
    None,
    Patrol,
    Escort,
    Haul,
    Salvage,
    Mining,
    StationDefense
};

struct FleetMember
{
    std::string ShipId;
    std::string Role;
    bool bActive = true;
};

struct FleetAssignment
{
    std::string AssignmentId;
    EFleetAssignmentType Type = EFleetAssignmentType::None;
    std::string TargetLocationId;
    std::string Notes;
};

struct FleetState
{
    std::string FleetId;
    std::string Name;
    std::vector<FleetMember> Members;
    std::vector<FleetAssignment> Assignments;
};

#include "Gameplay/Fleet/FleetSystem.h"
#include <iostream>

bool FleetSystem::Initialize()
{
    std::cout << "[Fleet] Initialize\n";
    return true;
}

void FleetSystem::RegisterFleet(const FleetState& Fleet)
{
    Fleets.push_back(Fleet);
    std::cout << "[Fleet] Registered " << Fleet.FleetId << "\n";
}

bool FleetSystem::AddMember(const std::string& FleetId, const FleetMember& Member)
{
    for (auto& Fleet : Fleets)
    {
        if (Fleet.FleetId == FleetId)
        {
            Fleet.Members.push_back(Member);
            std::cout << "[Fleet] Added member " << Member.ShipId << " to " << FleetId << "\n";
            return true;
        }
    }
    return false;
}

bool FleetSystem::AddAssignment(const std::string& FleetId, const FleetAssignment& Assignment)
{
    for (auto& Fleet : Fleets)
    {
        if (Fleet.FleetId == FleetId)
        {
            Fleet.Assignments.push_back(Assignment);
            std::cout << "[Fleet] Added assignment " << Assignment.AssignmentId << " to " << FleetId << "\n";
            return true;
        }
    }
    return false;
}

const FleetState* FleetSystem::FindFleet(const std::string& FleetId) const
{
    for (const auto& Fleet : Fleets)
    {
        if (Fleet.FleetId == FleetId)
        {
            return &Fleet;
        }
    }
    return nullptr;
}

void FleetSystem::Tick(float)
{
    std::cout << "[Fleet] Fleets=" << Fleets.size() << "\n";
}

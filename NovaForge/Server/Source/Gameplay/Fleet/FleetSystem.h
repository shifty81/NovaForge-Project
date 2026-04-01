#pragma once

#include "Gameplay/Fleet/FleetTypes.h"
#include <string>
#include <vector>

class FleetSystem
{
public:
    bool Initialize();
    void RegisterFleet(const FleetState& Fleet);
    bool AddMember(const std::string& FleetId, const FleetMember& Member);
    bool AddAssignment(const std::string& FleetId, const FleetAssignment& Assignment);
    const FleetState* FindFleet(const std::string& FleetId) const;
    void Tick(float DeltaTime);

private:
    std::vector<FleetState> Fleets;
};

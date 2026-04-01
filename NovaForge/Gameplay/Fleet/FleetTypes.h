// FleetTypes.h
// NovaForge fleet — data types for fleet composition, roles, and assignments.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Fleet
{

enum class EFleetRole : uint8_t
{
    Scout,
    Escort,
    Trader,
    Miner,
    Combat,
    Logistics,
    Capital
};

enum class EFleetStatus : uint8_t
{
    Idle,
    Patrolling,
    InTransit,
    Engaged,
    Docked,
    Destroyed
};

struct FleetShip
{
    uint64_t    shipId      = 0;
    std::string shipClass;
    EFleetRole  role        = EFleetRole::Scout;
    float       condition   = 1.0f; ///< 0–1 hull integrity
};

struct FleetRecord
{
    uint64_t                 fleetId    = 0;
    uint64_t                 ownerId    = 0;   ///< player or NPC faction ID
    std::string              name;
    EFleetStatus             status     = EFleetStatus::Idle;
    std::vector<FleetShip>   ships;
    std::string              currentSectorId;
    std::string              targetSectorId;
    float                    incomePerTick  = 0.0f;
    float                    riskLevel      = 0.0f; ///< 0–1 danger factor
};

struct FleetAssignment
{
    uint64_t fleetId   = 0;
    uint64_t missionInstanceId = 0;
    bool     active    = false;
};

} // namespace NovaForge::Gameplay::Fleet

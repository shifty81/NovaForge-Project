// FleetSystem.h
// NovaForge fleet — fleet management, assignment to contracts, and income/risk simulation.

#pragma once
#include "Fleet/FleetTypes.h"

#include <optional>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Fleet
{

class FleetSystem
{
public:
    FleetSystem()  = default;
    ~FleetSystem() = default;

    void initialise();
    void shutdown();

    // ---- fleet lifecycle -----------------------------------------------
    uint64_t createFleet(uint64_t ownerId, const std::string& name);
    bool     addShip(uint64_t fleetId, const FleetShip& ship);
    bool     removeShip(uint64_t fleetId, uint64_t shipId);
    bool     setStatus(uint64_t fleetId, EFleetStatus status);
    bool     moveTo(uint64_t fleetId, const std::string& sectorId);

    // ---- mission assignment --------------------------------------------
    bool     assignToMission(uint64_t fleetId, uint64_t missionInstanceId);
    bool     releaseFromMission(uint64_t fleetId);
    std::optional<FleetAssignment> getAssignment(uint64_t fleetId) const;

    // ---- income / risk simulation -------------------------------------
    /// Recalculate income and risk based on fleet role mix and sector danger.
    void     recalculateIncomeAndRisk(uint64_t fleetId,
                                      float sectorRiskModifier = 1.0f);

    /// Advance fleet simulation by one tick (income accrual, status updates).
    void     tick(float deltaSeconds);

    // ---- queries -------------------------------------------------------
    std::optional<const FleetRecord*> findFleet(uint64_t fleetId) const;
    std::vector<FleetRecord>          listFleets(uint64_t ownerId) const;
    std::vector<FleetRecord>          listByStatus(EFleetStatus status) const;

private:
    std::vector<FleetRecord>      fleets_;
    std::vector<FleetAssignment>  assignments_;
    uint64_t nextFleetId_ = 1;
    uint64_t nextShipId_  = 1;

    FleetRecord* getMutable(uint64_t fleetId);
};

} // namespace NovaForge::Gameplay::Fleet

// FleetSystem.cpp
// NovaForge fleet — fleet management, assignment to contracts, and income/risk simulation.

#include "Fleet/FleetSystem.h"

#include <algorithm>
#include <numeric>

namespace NovaForge::Gameplay::Fleet
{

void FleetSystem::initialise() {}
void FleetSystem::shutdown()   { fleets_.clear(); assignments_.clear(); }

uint64_t FleetSystem::createFleet(uint64_t ownerId, const std::string& name)
{
    FleetRecord rec;
    rec.fleetId = nextFleetId_++;
    rec.ownerId = ownerId;
    rec.name    = name;
    rec.status  = EFleetStatus::Idle;
    fleets_.push_back(rec);
    return rec.fleetId;
}

bool FleetSystem::addShip(uint64_t fleetId, const FleetShip& ship)
{
    FleetRecord* rec = getMutable(fleetId);
    if (!rec) return false;
    FleetShip s    = ship;
    s.shipId       = nextShipId_++;
    rec->ships.push_back(s);
    return true;
}

bool FleetSystem::removeShip(uint64_t fleetId, uint64_t shipId)
{
    FleetRecord* rec = getMutable(fleetId);
    if (!rec) return false;
    auto it = std::find_if(rec->ships.begin(), rec->ships.end(),
                           [shipId](const FleetShip& s){ return s.shipId == shipId; });
    if (it == rec->ships.end()) return false;
    rec->ships.erase(it);
    return true;
}

bool FleetSystem::setStatus(uint64_t fleetId, EFleetStatus status)
{
    FleetRecord* rec = getMutable(fleetId);
    if (!rec) return false;
    rec->status = status;
    return true;
}

bool FleetSystem::moveTo(uint64_t fleetId, const std::string& sectorId)
{
    FleetRecord* rec = getMutable(fleetId);
    if (!rec) return false;
    rec->targetSectorId = sectorId;
    rec->status         = EFleetStatus::InTransit;
    return true;
}

bool FleetSystem::assignToMission(uint64_t fleetId, uint64_t missionInstanceId)
{
    // Release any existing assignment first.
    releaseFromMission(fleetId);
    assignments_.push_back({ fleetId, missionInstanceId, true });
    return true;
}

bool FleetSystem::releaseFromMission(uint64_t fleetId)
{
    for (auto& a : assignments_)
    {
        if (a.fleetId == fleetId && a.active)
        {
            a.active = false;
            return true;
        }
    }
    return false;
}

std::optional<FleetAssignment> FleetSystem::getAssignment(uint64_t fleetId) const
{
    for (const auto& a : assignments_)
        if (a.fleetId == fleetId && a.active) return a;
    return std::nullopt;
}

void FleetSystem::recalculateIncomeAndRisk(uint64_t fleetId,
                                            float sectorRiskModifier)
{
    FleetRecord* rec = getMutable(fleetId);
    if (!rec) return;

    float baseIncome = 0.0f;
    float baseRisk   = 0.0f;

    for (const auto& ship : rec->ships)
    {
        switch (ship.role)
        {
            case EFleetRole::Trader:    baseIncome += 200.0f; baseRisk += 0.1f; break;
            case EFleetRole::Miner:     baseIncome += 150.0f; baseRisk += 0.05f; break;
            case EFleetRole::Combat:    baseIncome +=  80.0f; baseRisk += 0.3f; break;
            case EFleetRole::Escort:    baseIncome +=  50.0f; baseRisk += 0.15f; break;
            case EFleetRole::Logistics: baseIncome += 120.0f; baseRisk += 0.05f; break;
            case EFleetRole::Capital:   baseIncome += 500.0f; baseRisk += 0.4f; break;
            default:                    baseIncome +=  30.0f; baseRisk += 0.05f; break;
        }
    }

    rec->incomePerTick = baseIncome;
    rec->riskLevel     = std::min(1.0f, baseRisk * sectorRiskModifier);
}

void FleetSystem::tick(float /*deltaSeconds*/)
{
    for (auto& fleet : fleets_)
    {
        // Arrive at target if in-transit (stub: instant arrival).
        if (fleet.status == EFleetStatus::InTransit &&
            !fleet.targetSectorId.empty())
        {
            fleet.currentSectorId = fleet.targetSectorId;
            fleet.targetSectorId.clear();
            fleet.status = EFleetStatus::Idle;
        }
    }
}

std::optional<const FleetRecord*> FleetSystem::findFleet(uint64_t fleetId) const
{
    for (const auto& f : fleets_)
        if (f.fleetId == fleetId) return &f;
    return std::nullopt;
}

std::vector<FleetRecord> FleetSystem::listFleets(uint64_t ownerId) const
{
    std::vector<FleetRecord> result;
    for (const auto& f : fleets_)
        if (f.ownerId == ownerId) result.push_back(f);
    return result;
}

std::vector<FleetRecord> FleetSystem::listByStatus(EFleetStatus status) const
{
    std::vector<FleetRecord> result;
    for (const auto& f : fleets_)
        if (f.status == status) result.push_back(f);
    return result;
}

FleetRecord* FleetSystem::getMutable(uint64_t fleetId)
{
    for (auto& f : fleets_)
        if (f.fleetId == fleetId) return &f;
    return nullptr;
}

} // namespace NovaForge::Gameplay::Fleet

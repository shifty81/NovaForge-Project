// WorldSimSystem.cpp
// NovaForge world-simulation — sector control, anomalies, war, titan-race pressure.

#include "WorldSim/WorldSimSystem.h"

#include <algorithm>

namespace NovaForge::Gameplay::WorldSim
{

void WorldSimSystem::initialise() {}
void WorldSimSystem::shutdown()   { sectors_.clear(); anomalies_.clear(); }

void WorldSimSystem::registerSector(const SectorState& sector)
{
    sectors_.push_back(sector);
}

bool WorldSimSystem::updateSectorControl(const std::string& sectorId,
                                          ESectorControl control,
                                          uint32_t       factionId)
{
    SectorState* s = getMutableSector(sectorId);
    if (!s) return false;
    s->control             = control;
    s->controllingFaction  = factionId;
    recalculateSectorState(sectorId);
    return true;
}

bool WorldSimSystem::setWarActive(const std::string& sectorId, bool warActive)
{
    SectorState* s = getMutableSector(sectorId);
    if (!s) return false;
    s->warActive = warActive;
    recalculateSectorState(sectorId);
    return true;
}

void WorldSimSystem::recalculateSectorState(const std::string& sectorId)
{
    SectorState* s = getMutableSector(sectorId);
    if (!s) return;

    // Contested / war zones increase danger and opportunity.
    if (s->warActive)
    {
        s->dangerLevel      = std::min(1.0f, s->dangerLevel + 0.3f);
        s->opportunityLevel = std::min(2.0f, s->opportunityLevel + 0.5f);
    }

    // Count active anomalies in this sector.
    int32_t anomalyCount = 0;
    for (const auto& a : anomalies_)
        if (a.sectorId == sectorId && a.active) ++anomalyCount;

    if (anomalyCount > 0)
        s->opportunityLevel = std::min(2.0f,
            s->opportunityLevel + 0.2f * static_cast<float>(anomalyCount));
}

std::optional<SectorState>
WorldSimSystem::findSector(const std::string& sectorId) const
{
    for (const auto& s : sectors_)
        if (s.sectorId == sectorId) return s;
    return std::nullopt;
}

std::vector<SectorState> WorldSimSystem::listSectors() const { return sectors_; }

std::vector<SectorState> WorldSimSystem::listWarZones() const
{
    std::vector<SectorState> result;
    for (const auto& s : sectors_)
        if (s.warActive) result.push_back(s);
    return result;
}

void WorldSimSystem::spawnAnomaly(const AnomalyEvent& anomaly)
{
    anomalies_.push_back(anomaly);
    recalculateSectorState(anomaly.sectorId);
}

bool WorldSimSystem::resolveAnomaly(const std::string& anomalyId)
{
    for (auto& a : anomalies_)
    {
        if (a.anomalyId == anomalyId && a.active)
        {
            a.active = false;
            recalculateSectorState(a.sectorId);
            return true;
        }
    }
    return false;
}

std::vector<AnomalyEvent>
WorldSimSystem::listActiveAnomalies(const std::string& sectorId) const
{
    std::vector<AnomalyEvent> result;
    for (const auto& a : anomalies_)
        if (a.sectorId == sectorId && a.active) result.push_back(a);
    return result;
}

void WorldSimSystem::advanceTitanPhase()
{
    if (m_titanState.currentPhase < 5) ++m_titanState.currentPhase;
    m_titanState.progressFraction = 0.0f;
    m_titanState.globalPressure   =
        static_cast<float>(m_titanState.currentPhase) / 5.0f;
}

void WorldSimSystem::setSeasonActive(bool active)
{
    m_titanState.seasonActive = active;
}

void WorldSimSystem::tick(float /*deltaSeconds*/)
{
    // Stub: advance titan race progress, expire old anomalies, etc.
}

SectorState* WorldSimSystem::getMutableSector(const std::string& sectorId)
{
    for (auto& s : sectors_)
        if (s.sectorId == sectorId) return &s;
    return nullptr;
}

} // namespace NovaForge::Gameplay::WorldSim

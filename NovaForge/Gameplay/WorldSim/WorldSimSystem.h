// WorldSimSystem.h
// NovaForge world-simulation — sector control, anomalies, war, titan-race pressure.

#pragma once
#include "WorldSim/WorldSimTypes.h"

#include <optional>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::WorldSim
{

class WorldSimSystem
{
public:
    WorldSimSystem()  = default;
    ~WorldSimSystem() = default;

    void initialise();
    void shutdown();

    // ---- sector management --------------------------------------------
    void    registerSector(const SectorState& sector);
    bool    updateSectorControl(const std::string& sectorId,
                                 ESectorControl control,
                                 uint32_t       factionId);
    bool    setWarActive(const std::string& sectorId, bool warActive);

    /// Recalculate opportunity and danger levels for a sector after events.
    void    recalculateSectorState(const std::string& sectorId);

    std::optional<SectorState>     findSector(const std::string& sectorId) const;
    std::vector<SectorState>        listSectors() const;
    std::vector<SectorState>        listWarZones() const;

    // ---- anomalies ----------------------------------------------------
    void    spawnAnomaly(const AnomalyEvent& anomaly);
    bool    resolveAnomaly(const std::string& anomalyId);
    std::vector<AnomalyEvent> listActiveAnomalies(const std::string& sectorId) const;

    // ---- titan race / season ------------------------------------------
    void    advanceTitanPhase();
    void    setSeasonActive(bool active);
    float   getGlobalPressure() const { return m_titanState.globalPressure; }
    const TitanRaceState& getTitanState() const { return m_titanState; }

    // ---- tick ---------------------------------------------------------
    void    tick(float deltaSeconds);

private:
    std::vector<SectorState>  sectors_;
    std::vector<AnomalyEvent> anomalies_;
    TitanRaceState            m_titanState;

    SectorState* getMutableSector(const std::string& sectorId);
};

} // namespace NovaForge::Gameplay::WorldSim

// WorldSimTypes.h
// NovaForge world-simulation — sector state, anomalies, war zones, titan race pressure.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::WorldSim
{

enum class ESectorControl : uint8_t { Contested, FactionA, FactionB, Independent, Unclaimed };

struct SectorState
{
    std::string     sectorId;
    ESectorControl  control          = ESectorControl::Unclaimed;
    float           opportunityLevel = 1.0f; ///< 0–2; affects contract quality
    float           dangerLevel      = 0.2f; ///< 0–1
    bool            warActive        = false;
    uint32_t        controllingFaction = 0;  ///< 0 = none
};

enum class EAnomalyType : uint8_t { ResourceBurst, RiftEvent, PirateIncursion, TitanFragment };

struct AnomalyEvent
{
    std::string  anomalyId;
    std::string  sectorId;
    EAnomalyType type            = EAnomalyType::ResourceBurst;
    float        magnitude       = 1.0f;
    bool         active          = true;
};

struct TitanRaceState
{
    uint32_t    currentPhase     = 1;    ///< 1–5 escalating pressure
    float       progressFraction = 0.0f; ///< 0–1 within the current phase
    float       globalPressure   = 0.0f; ///< derived urgency factor
    bool        seasonActive     = false;
};

} // namespace NovaForge::Gameplay::WorldSim

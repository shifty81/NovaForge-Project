// AnomalySystem.h
// NovaForge Gameplay — anomaly system: resource/encounter modifications,
// anomaly spawning, duration, and effect propagation.

#pragma once
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace novaforge::gameplay {

enum class EAnomalyType : uint8_t
{
    ResourceRich,       ///< bonus mineral/gas yield in sector
    ResourceDepleted,   ///< reduced resource yield
    EncounterDense,     ///< more enemy/NPC spawns
    EncounterSparse,    ///< fewer encounters
    StormZone,          ///< navigation/combat hazard
    SignalAnomaly,       ///< mystery scan event
    TitanFragment,       ///< titan race artefact site
    WarpRift,            ///< fast-travel shortcut or hazard
};

struct AnomalyEntry
{
    std::string   anomalyId;
    EAnomalyType  type            = EAnomalyType::ResourceRich;
    std::string   sectorId;
    float         intensity       = 1.0f;   ///< multiplier strength
    float         remainingSeconds = 300.f;  ///< 0 = permanent
    bool          isPermanent     = false;
    bool          isActive        = true;
    std::string   sourceTag;   ///< "titan_race", "pcg_event", "player_action"
};

struct AnomalyEffect
{
    float  resourceYieldMultiplier   = 1.f;
    float  encounterRateMultiplier   = 1.f;
    float  navigationHazardLevel     = 0.f;  ///< 0–1
    bool   isWarpRift                = false;
};

class AnomalySystem
{
public:
    bool Initialize();
    void Shutdown();

    // ---- anomaly management -----------------------------------------
    void  SpawnAnomaly  (const AnomalyEntry& anomaly);
    bool  DespawnAnomaly(const std::string& anomalyId);
    bool  HasAnomaly    (const std::string& anomalyId) const;
    std::optional<AnomalyEntry> FindAnomaly(const std::string& anomalyId) const;

    // ---- queries per sector -----------------------------------------
    std::vector<AnomalyEntry> GetAnomaliesInSector(const std::string& sectorId) const;
    AnomalyEffect             ComputeSectorEffect  (const std::string& sectorId) const;

    // ---- tick (duration management) ---------------------------------
    void Tick(float deltaSeconds);

    // ---- callbacks --------------------------------------------------
    using AnomalyExpiredCallback = std::function<void(const std::string& anomalyId)>;
    void SetExpiredCallback(AnomalyExpiredCallback cb)
    { m_expiredCb = std::move(cb); }

    // ---- list all ---------------------------------------------------
    const std::vector<AnomalyEntry>& GetAll() const { return m_anomalies; }
    size_t ActiveCount() const;

private:
    std::vector<AnomalyEntry>   m_anomalies;
    AnomalyExpiredCallback      m_expiredCb;

    AnomalyEntry* GetMutable(const std::string& anomalyId);
};

} // namespace novaforge::gameplay

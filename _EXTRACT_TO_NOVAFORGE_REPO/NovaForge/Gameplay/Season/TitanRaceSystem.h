// TitanRaceSystem.h
// NovaForge Gameplay — season + titan race meta-game: pressure levels,
// season phases, titan progress tracking, and long-term world effects.

#pragma once
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace novaforge::gameplay {

// ---------------------------------------------------------------------------
// Season phase
// ---------------------------------------------------------------------------

enum class ESeasonPhase : uint8_t
{
    Dormant,      ///< titan race not yet active
    EarlyRace,    ///< titans discovered, pressure building
    MidRace,      ///< contested sectors, anomalies active
    LateRace,     ///< high pressure, war/conflict escalating
    FinalPush,    ///< climactic phase
    SeasonEnd,    ///< resolution, rewards, reset
};

// ---------------------------------------------------------------------------
// Titan entry
// ---------------------------------------------------------------------------

struct TitanEntry
{
    std::string  titanId;
    std::string  name;
    std::string  factionId;
    float        progress     = 0.f;   ///< 0–100 race progress
    float        pressure     = 0.f;   ///< 0–100 global pressure contribution
    bool         isActive     = true;
    bool         isDefeated   = false;
};

// ---------------------------------------------------------------------------
// Season config
// ---------------------------------------------------------------------------

struct SeasonConfig
{
    uint32_t    seasonNumber   = 1;
    std::string seasonName;
    float       phaseThresholds[6] = { 0.f, 10.f, 30.f, 60.f, 85.f, 100.f };
    float       pressureDecayRate  = 0.01f;  ///< per tick decay
    float       pressureBuildRate  = 0.5f;   ///< per titan per tick
};

// ---------------------------------------------------------------------------
// TitanRaceSystem
// ---------------------------------------------------------------------------

class TitanRaceSystem
{
public:
    bool Initialize(const SeasonConfig& config);
    void Shutdown();

    // ---- titan management ----------------------------------------
    void RegisterTitan(const TitanEntry& titan);
    bool AdvanceTitan (const std::string& titanId, float progressDelta);
    bool DefeatTitan  (const std::string& titanId);
    std::optional<TitanEntry> FindTitan(const std::string& titanId) const;
    const std::vector<TitanEntry>& GetTitans() const { return m_titans; }

    // ---- season state -----------------------------------------------
    void          Tick(float deltaSeconds);
    ESeasonPhase  GetPhase()        const { return m_phase; }
    float         GetGlobalPressure() const { return m_globalPressure; }
    uint32_t      GetSeasonNumber() const { return m_config.seasonNumber; }
    bool          IsSeasonOver()    const { return m_phase == ESeasonPhase::SeasonEnd; }

    // ---- effect queries ---------------------------------------------
    /// Combat difficulty modifier (1.0 = normal, higher = harder).
    float GetCombatDifficultyModifier() const;
    /// Resource spawn modifier (1.0 = normal).
    float GetResourceSpawnModifier() const;
    /// Contract reward multiplier.
    float GetContractRewardMultiplier() const;

    // ---- callbacks --------------------------------------------------
    using PhaseChangedCallback = std::function<void(ESeasonPhase prev,
                                                     ESeasonPhase next)>;
    void SetPhaseChangedCallback(PhaseChangedCallback cb)
    { m_phaseCb = std::move(cb); }

    // ---- season end reset ------------------------------------------
    void AdvanceSeason();

private:
    SeasonConfig               m_config;
    std::vector<TitanEntry>    m_titans;
    ESeasonPhase               m_phase         = ESeasonPhase::Dormant;
    float                      m_globalPressure = 0.f;
    float                      m_tickAccum      = 0.f;
    PhaseChangedCallback       m_phaseCb;

    void UpdatePhase();
    TitanEntry* GetMutable(const std::string& titanId);
};

// ---------------------------------------------------------------------------
// SeasonSystem (thin wrapper for season-level state)
// ---------------------------------------------------------------------------

class SeasonSystem
{
public:
    bool Initialize();
    void Shutdown();

    uint32_t  CurrentSeason() const  { return m_season; }
    void      IncrementSeason()      { ++m_season; }
    void      SetSeason(uint32_t n)  { m_season = n; }

    // Season-wide modifiers accumulated from all sources.
    void  AddContractMultiplier(float delta)   { m_contractMult += delta; }
    void  AddCombatDifficultyDelta(float delta){ m_combatDiff   += delta; }
    float GetContractMultiplier()  const       { return m_contractMult; }
    float GetCombatDifficulty()    const       { return m_combatDiff; }
    void  ResetMultipliers();

private:
    uint32_t m_season        = 1;
    float    m_contractMult  = 1.f;
    float    m_combatDiff    = 1.f;
};

} // namespace novaforge::gameplay

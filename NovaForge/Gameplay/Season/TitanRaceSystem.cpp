// TitanRaceSystem.cpp
// NovaForge Gameplay — titan race + season system.

#include "Season/TitanRaceSystem.h"

#include <algorithm>
#include <cmath>

namespace novaforge::gameplay {

bool TitanRaceSystem::Initialize(const SeasonConfig& config)
{
    m_config          = config;
    m_phase           = ESeasonPhase::Dormant;
    m_globalPressure  = 0.f;
    m_tickAccum       = 0.f;
    return true;
}

void TitanRaceSystem::Shutdown()
{
    m_titans.clear();
}

void TitanRaceSystem::RegisterTitan(const TitanEntry& titan)
{
    for (auto& t : m_titans)
        if (t.titanId == titan.titanId) { t = titan; return; }
    m_titans.push_back(titan);
}

bool TitanRaceSystem::AdvanceTitan(const std::string& titanId,
                                     float progressDelta)
{
    TitanEntry* t = GetMutable(titanId);
    if (!t || !t->isActive || t->isDefeated) return false;
    t->progress += progressDelta;
    if (t->progress > 100.f) t->progress = 100.f;
    return true;
}

bool TitanRaceSystem::DefeatTitan(const std::string& titanId)
{
    TitanEntry* t = GetMutable(titanId);
    if (!t) return false;
    t->isDefeated = true;
    t->isActive   = false;
    return true;
}

std::optional<TitanEntry> TitanRaceSystem::FindTitan(
    const std::string& titanId) const
{
    for (const auto& t : m_titans)
        if (t.titanId == titanId) return t;
    return std::nullopt;
}

void TitanRaceSystem::Tick(float deltaSeconds)
{
    m_tickAccum += deltaSeconds;

    // Rebuild global pressure from active titans.
    float totalPressure = 0.f;
    for (auto& t : m_titans)
    {
        if (!t.isActive || t.isDefeated) continue;
        t.pressure += m_config.pressureBuildRate * deltaSeconds;
        if (t.pressure > 100.f) t.pressure = 100.f;
        totalPressure += t.pressure;
    }

    float numActive = 0.f;
    for (const auto& t : m_titans)
        if (t.isActive && !t.isDefeated) numActive += 1.f;

    if (numActive > 0.f)
        m_globalPressure = totalPressure / numActive;

    // Decay if no active titans.
    if (numActive == 0.f)
        m_globalPressure -= m_config.pressureDecayRate * deltaSeconds;
    if (m_globalPressure < 0.f) m_globalPressure = 0.f;

    UpdatePhase();
}

void TitanRaceSystem::UpdatePhase()
{
    ESeasonPhase newPhase = ESeasonPhase::Dormant;
    const float* t = m_config.phaseThresholds;

    if      (m_globalPressure >= t[5]) newPhase = ESeasonPhase::SeasonEnd;
    else if (m_globalPressure >= t[4]) newPhase = ESeasonPhase::FinalPush;
    else if (m_globalPressure >= t[3]) newPhase = ESeasonPhase::LateRace;
    else if (m_globalPressure >= t[2]) newPhase = ESeasonPhase::MidRace;
    else if (m_globalPressure >= t[1]) newPhase = ESeasonPhase::EarlyRace;
    else                                newPhase = ESeasonPhase::Dormant;

    if (newPhase != m_phase)
    {
        ESeasonPhase prev = m_phase;
        m_phase = newPhase;
        if (m_phaseCb) m_phaseCb(prev, m_phase);
    }
}

float TitanRaceSystem::GetCombatDifficultyModifier() const
{
    return 1.f + m_globalPressure / 200.f;  // up to +50% at max pressure
}

float TitanRaceSystem::GetResourceSpawnModifier() const
{
    return 1.f - m_globalPressure / 300.f;  // down to -33% at max pressure
}

float TitanRaceSystem::GetContractRewardMultiplier() const
{
    return 1.f + m_globalPressure / 100.f;  // up to 2× at max pressure
}

void TitanRaceSystem::AdvanceSeason()
{
    ++m_config.seasonNumber;
    m_globalPressure = 0.f;
    m_phase          = ESeasonPhase::Dormant;
    m_titans.clear();
}

TitanEntry* TitanRaceSystem::GetMutable(const std::string& titanId)
{
    for (auto& t : m_titans)
        if (t.titanId == titanId) return &t;
    return nullptr;
}

bool SeasonSystem::Initialize()
{
    m_season       = 1;
    m_contractMult = 1.f;
    m_combatDiff   = 1.f;
    return true;
}

void SeasonSystem::Shutdown() {}

void SeasonSystem::ResetMultipliers()
{
    m_contractMult = 1.f;
    m_combatDiff   = 1.f;
}

} // namespace novaforge::gameplay

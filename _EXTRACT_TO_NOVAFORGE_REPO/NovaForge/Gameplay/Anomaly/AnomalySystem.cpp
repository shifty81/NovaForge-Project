// AnomalySystem.cpp
// NovaForge Gameplay — anomaly system.

#include "Anomaly/AnomalySystem.h"

#include <algorithm>

namespace novaforge::gameplay {

bool AnomalySystem::Initialize() { return true; }
void AnomalySystem::Shutdown()   { m_anomalies.clear(); }

void AnomalySystem::SpawnAnomaly(const AnomalyEntry& anomaly)
{
    for (auto& a : m_anomalies)
        if (a.anomalyId == anomaly.anomalyId) { a = anomaly; return; }
    m_anomalies.push_back(anomaly);
}

bool AnomalySystem::DespawnAnomaly(const std::string& anomalyId)
{
    auto it = std::find_if(m_anomalies.begin(), m_anomalies.end(),
                           [&](const AnomalyEntry& a){ return a.anomalyId == anomalyId; });
    if (it == m_anomalies.end()) return false;
    m_anomalies.erase(it);
    return true;
}

bool AnomalySystem::HasAnomaly(const std::string& anomalyId) const
{
    return FindAnomaly(anomalyId).has_value();
}

std::optional<AnomalyEntry> AnomalySystem::FindAnomaly(
    const std::string& anomalyId) const
{
    for (const auto& a : m_anomalies)
        if (a.anomalyId == anomalyId) return a;
    return std::nullopt;
}

std::vector<AnomalyEntry> AnomalySystem::GetAnomaliesInSector(
    const std::string& sectorId) const
{
    std::vector<AnomalyEntry> result;
    for (const auto& a : m_anomalies)
        if (a.sectorId == sectorId && a.isActive)
            result.push_back(a);
    return result;
}

AnomalyEffect AnomalySystem::ComputeSectorEffect(
    const std::string& sectorId) const
{
    AnomalyEffect effect;
    for (const auto& a : m_anomalies)
    {
        if (a.sectorId != sectorId || !a.isActive) continue;
        float i = a.intensity;
        switch (a.type)
        {
            case EAnomalyType::ResourceRich:
                effect.resourceYieldMultiplier *= (1.f + 0.5f * i);
                break;
            case EAnomalyType::ResourceDepleted:
                effect.resourceYieldMultiplier *= (1.f - 0.4f * i);
                if (effect.resourceYieldMultiplier < 0.1f)
                    effect.resourceYieldMultiplier = 0.1f;
                break;
            case EAnomalyType::EncounterDense:
                effect.encounterRateMultiplier *= (1.f + 0.8f * i);
                break;
            case EAnomalyType::EncounterSparse:
                effect.encounterRateMultiplier *= (1.f - 0.5f * i);
                if (effect.encounterRateMultiplier < 0.1f)
                    effect.encounterRateMultiplier = 0.1f;
                break;
            case EAnomalyType::StormZone:
                effect.navigationHazardLevel += 0.4f * i;
                if (effect.navigationHazardLevel > 1.f)
                    effect.navigationHazardLevel = 1.f;
                break;
            case EAnomalyType::WarpRift:
                effect.isWarpRift = true;
                break;
            default:
                break;
        }
    }
    return effect;
}

void AnomalySystem::Tick(float deltaSeconds)
{
    for (auto& a : m_anomalies)
    {
        if (a.isPermanent || !a.isActive) continue;
        a.remainingSeconds -= deltaSeconds;
        if (a.remainingSeconds <= 0.f)
        {
            a.isActive         = false;
            a.remainingSeconds = 0.f;
            if (m_expiredCb) m_expiredCb(a.anomalyId);
        }
    }

    // Prune inactive, non-permanent anomalies.
    m_anomalies.erase(
        std::remove_if(m_anomalies.begin(), m_anomalies.end(),
                       [](const AnomalyEntry& a){
                           return !a.isActive && !a.isPermanent; }),
        m_anomalies.end());
}

size_t AnomalySystem::ActiveCount() const
{
    size_t c = 0;
    for (const auto& a : m_anomalies)
        if (a.isActive) ++c;
    return c;
}

AnomalyEntry* AnomalySystem::GetMutable(const std::string& anomalyId)
{
    for (auto& a : m_anomalies)
        if (a.anomalyId == anomalyId) return &a;
    return nullptr;
}

} // namespace novaforge::gameplay

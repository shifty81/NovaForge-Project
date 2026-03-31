// WarSectorSystem.cpp
// NovaForge Gameplay — war/sector conflict system.

#include "WarSector/WarSectorSystem.h"

#include <algorithm>

namespace novaforge::gameplay {

bool WarSectorSystem::Initialize() { return true; }
void WarSectorSystem::Shutdown()   { m_sectors.clear(); }

void WarSectorSystem::RegisterSector(const SectorWarState& sector)
{
    for (auto& s : m_sectors)
        if (s.sectorId == sector.sectorId) { s = sector; return; }
    m_sectors.push_back(sector);
}

bool WarSectorSystem::UnregisterSector(const std::string& sectorId)
{
    auto it = std::find_if(m_sectors.begin(), m_sectors.end(),
                           [&](const SectorWarState& s){ return s.sectorId == sectorId; });
    if (it == m_sectors.end()) return false;
    m_sectors.erase(it);
    return true;
}

bool WarSectorSystem::HasSector(const std::string& sectorId) const
{
    return FindSector(sectorId).has_value();
}

std::optional<SectorWarState> WarSectorSystem::FindSector(
    const std::string& sectorId) const
{
    for (const auto& s : m_sectors)
        if (s.sectorId == sectorId) return s;
    return std::nullopt;
}

bool WarSectorSystem::DeclareWar(const WarDeclaration& decl)
{
    SectorWarState* s = GetMutable(decl.triggerSectorId);
    if (!s) return false;
    EConflictGrade prev = s->conflictGrade;
    s->attackerFactionId   = decl.aggressorFactionId;
    s->defenderFactionId   = decl.defenderFactionId;
    s->conflictIntensity   = decl.declaredConflictIntensity;
    s->control             = ESectorControl::Contested;
    RecomputeGrade(*s);
    if (s->conflictGrade != prev && m_conflictCb)
        m_conflictCb(s->sectorId, s->conflictGrade);
    return true;
}

bool WarSectorSystem::EscalateConflict(const std::string& sectorId,
                                          float intensityDelta)
{
    SectorWarState* s = GetMutable(sectorId);
    if (!s) return false;
    EConflictGrade prev = s->conflictGrade;
    s->conflictIntensity += intensityDelta;
    if (s->conflictIntensity > 100.f) s->conflictIntensity = 100.f;
    RecomputeGrade(*s);
    if (s->conflictGrade != prev && m_conflictCb)
        m_conflictCb(s->sectorId, s->conflictGrade);
    return true;
}

bool WarSectorSystem::ReduceConflict(const std::string& sectorId,
                                        float intensityDelta)
{
    SectorWarState* s = GetMutable(sectorId);
    if (!s) return false;
    EConflictGrade prev = s->conflictGrade;
    s->conflictIntensity -= intensityDelta;
    if (s->conflictIntensity < 0.f) s->conflictIntensity = 0.f;
    RecomputeGrade(*s);
    if (s->conflictGrade != prev && m_conflictCb)
        m_conflictCb(s->sectorId, s->conflictGrade);
    return true;
}

bool WarSectorSystem::ResolveConflict(const std::string& sectorId,
                                         const std::string& winnerFactionId)
{
    SectorWarState* s = GetMutable(sectorId);
    if (!s) return false;
    s->conflictIntensity   = 0.f;
    s->controlFactionId    = winnerFactionId;
    s->control             = ESectorControl::FactionA; // winner takes control
    s->attackerFactionId.clear();
    s->defenderFactionId.clear();
    EConflictGrade prev    = s->conflictGrade;
    s->conflictGrade       = EConflictGrade::Peace;
    s->turnsInConflict     = 0;
    if (prev != EConflictGrade::Peace && m_conflictCb)
        m_conflictCb(sectorId, EConflictGrade::Peace);
    return true;
}

bool WarSectorSystem::SetSectorControl(const std::string& sectorId,
                                          ESectorControl control,
                                          const std::string& factionId)
{
    SectorWarState* s = GetMutable(sectorId);
    if (!s) return false;
    s->control           = control;
    s->controlFactionId  = factionId;
    return true;
}

float WarSectorSystem::GetOpportunityLevel(const std::string& sectorId) const
{
    for (const auto& s : m_sectors)
        if (s.sectorId == sectorId) return s.opportunityLevel;
    return 0.f;
}

void WarSectorSystem::UpdateOpportunityLevels()
{
    for (auto& s : m_sectors)
    {
        // Opportunity scales with conflict intensity and grade.
        float base = s.conflictIntensity / 100.f;
        float gradeBonus = static_cast<float>(s.conflictGrade) * 0.15f;
        s.opportunityLevel = base + gradeBonus;
        if (s.opportunityLevel > 1.f) s.opportunityLevel = 1.f;
    }
}

std::vector<SectorWarState> WarSectorSystem::GetConflictingSectors() const
{
    std::vector<SectorWarState> result;
    for (const auto& s : m_sectors)
        if (s.conflictGrade != EConflictGrade::Peace)
            result.push_back(s);
    return result;
}

std::vector<SectorWarState> WarSectorSystem::GetSectorsByFaction(
    const std::string& factionId) const
{
    std::vector<SectorWarState> result;
    for (const auto& s : m_sectors)
        if (s.controlFactionId == factionId ||
            s.attackerFactionId == factionId ||
            s.defenderFactionId == factionId)
            result.push_back(s);
    return result;
}

void WarSectorSystem::Tick(float /*deltaSeconds*/)
{
    for (auto& s : m_sectors)
    {
        if (s.conflictGrade != EConflictGrade::Peace)
            ++s.turnsInConflict;
    }
    UpdateOpportunityLevels();
}

size_t WarSectorSystem::ActiveWarCount() const
{
    size_t c = 0;
    for (const auto& s : m_sectors)
        if (s.conflictGrade >= EConflictGrade::War) ++c;
    return c;
}

void WarSectorSystem::RecomputeGrade(SectorWarState& sector)
{
    float i = sector.conflictIntensity;
    if      (i >= 80.f) sector.conflictGrade = EConflictGrade::TotalWar;
    else if (i >= 60.f) sector.conflictGrade = EConflictGrade::War;
    else if (i >= 35.f) sector.conflictGrade = EConflictGrade::Skirmish;
    else if (i >= 10.f) sector.conflictGrade = EConflictGrade::Tension;
    else                sector.conflictGrade = EConflictGrade::Peace;
}

SectorWarState* WarSectorSystem::GetMutable(const std::string& sectorId)
{
    for (auto& s : m_sectors)
        if (s.sectorId == sectorId) return &s;
    return nullptr;
}

} // namespace novaforge::gameplay

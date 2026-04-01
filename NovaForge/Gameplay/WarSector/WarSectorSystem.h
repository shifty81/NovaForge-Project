// WarSectorSystem.h
// NovaForge Gameplay — war/sector conflict system: conflict grades,
// opportunity levels, faction war declarations, sector control tracking.

#pragma once
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace novaforge::gameplay {

enum class EConflictGrade : uint8_t
{
    Peace,         ///< no active conflict
    Tension,       ///< elevated threat, skirmishes
    Skirmish,      ///< minor active fighting
    War,           ///< full-scale faction war
    TotalWar,      ///< existential conflict, sector destabilised
};

enum class ESectorControl : uint8_t
{
    Neutral,
    FactionA,
    FactionB,
    Contested,
};

struct SectorWarState
{
    std::string     sectorId;
    EConflictGrade  conflictGrade      = EConflictGrade::Peace;
    ESectorControl  control            = ESectorControl::Neutral;
    std::string     controlFactionId;   ///< dominant faction
    std::string     attackerFactionId;
    std::string     defenderFactionId;
    float           conflictIntensity  = 0.f;   ///< 0–100
    float           opportunityLevel   = 0.f;   ///< bounty/contract bonus factor
    uint32_t        turnsInConflict    = 0;
};

struct WarDeclaration
{
    std::string aggressorFactionId;
    std::string defenderFactionId;
    std::string triggerSectorId;
    float       declaredConflictIntensity = 50.f;
};

class WarSectorSystem
{
public:
    bool Initialize();
    void Shutdown();

    // ---- sector registration ----------------------------------------
    void  RegisterSector (const SectorWarState& sector);
    bool  UnregisterSector(const std::string& sectorId);
    bool  HasSector       (const std::string& sectorId) const;
    std::optional<SectorWarState> FindSector(const std::string& sectorId) const;
    const std::vector<SectorWarState>& GetAllSectors() const { return m_sectors; }

    // ---- war declaration / escalation --------------------------------
    bool DeclareWar    (const WarDeclaration& decl);
    bool EscalateConflict(const std::string& sectorId, float intensityDelta);
    bool ReduceConflict (const std::string& sectorId, float intensityDelta);
    bool ResolveConflict(const std::string& sectorId,
                          const std::string& winnerFactionId);

    // ---- sector control change ----------------------------------------
    bool SetSectorControl(const std::string& sectorId,
                           ESectorControl control,
                           const std::string& factionId = "");

    // ---- opportunity level -------------------------------------------
    float GetOpportunityLevel(const std::string& sectorId) const;
    void  UpdateOpportunityLevels();

    // ---- queries -------------------------------------------------------
    std::vector<SectorWarState> GetConflictingSectors() const;
    std::vector<SectorWarState> GetSectorsByFaction  (const std::string& factionId) const;

    // ---- tick ----------------------------------------------------------
    void Tick(float deltaSeconds);

    // ---- callbacks -----------------------------------------------------
    using ConflictChangedCallback = std::function<void(const std::string& sectorId,
                                                         EConflictGrade grade)>;
    void SetConflictChangedCallback(ConflictChangedCallback cb)
    { m_conflictCb = std::move(cb); }

    size_t ActiveWarCount() const;

private:
    std::vector<SectorWarState>  m_sectors;
    ConflictChangedCallback      m_conflictCb;

    SectorWarState* GetMutable(const std::string& sectorId);
    void            RecomputeGrade(SectorWarState& sector);
};

} // namespace novaforge::gameplay

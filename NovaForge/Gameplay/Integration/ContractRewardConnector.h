// ContractRewardConnector.h
// NovaForge Integration — connects contract completion to credits, faction
// standing rewards, skill XP, and gates better contracts by standing/skill level.

#pragma once
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace novaforge::integration {

// ---------------------------------------------------------------------------
// Skill XP gain event
// ---------------------------------------------------------------------------

struct SkillXPEvent
{
    uint64_t    playerId    = 0;
    std::string skillId;      ///< e.g. "combat", "salvage", "trading", "exploration"
    float       xpGained    = 0.f;
};

// ---------------------------------------------------------------------------
// Faction standing change
// ---------------------------------------------------------------------------

struct StandingChangeEvent
{
    uint64_t    playerId    = 0;
    uint32_t    factionId   = 0;
    std::string factionName;
    float       delta       = 0.f;   ///< positive = improved
    float       newStanding = 0.f;
};

// ---------------------------------------------------------------------------
// Contract reward definition
// ---------------------------------------------------------------------------

struct ContractRewardDefinition
{
    std::string   contractId;
    std::string   contractType;     ///< "combat", "salvage", "delivery", "exploration"
    float         baseCreditReward = 0.f;
    float         factionStandingDelta = 0.f;
    uint32_t      primaryFactionId    = 0;
    float         skillXPAmount       = 0.f;
    std::string   rewardSkillId;
    float         requiredStanding    = -10.f;  ///< minimum faction standing to accept
    float         requiredSkillLevel  = 0.f;    ///< minimum skill level to accept (0 = any)
    std::string   requiredSkillId;
};

// ---------------------------------------------------------------------------
// Contract eligibility result
// ---------------------------------------------------------------------------

struct ContractEligibility
{
    bool        eligible              = false;
    std::string failReason;
    float       playerStanding        = 0.f;
    float       playerSkillLevel      = 0.f;
};

// ---------------------------------------------------------------------------
// Callbacks
// ---------------------------------------------------------------------------

using CreditRewardCallback  = std::function<void(uint64_t playerId, float credits, const std::string& reason)>;
using StandingRewardCallback = std::function<void(const StandingChangeEvent&)>;
using SkillXPCallback        = std::function<void(const SkillXPEvent&)>;

// ---------------------------------------------------------------------------
// ContractRewardConnector
// ---------------------------------------------------------------------------

class ContractRewardConnector
{
public:
    bool Initialize();
    void Shutdown();

    // ---- contract reward registration ----------------------------------
    void RegisterContract   (const ContractRewardDefinition& def);
    bool UnregisterContract (const std::string& contractId);
    bool HasContract        (const std::string& contractId) const;
    std::optional<ContractRewardDefinition> FindContract(
        const std::string& contractId) const;
    size_t ContractCount() const { return m_contracts.size(); }

    // ---- contract completion -------------------------------------------
    /// Execute all rewards for a completed contract.
    /// Returns credit amount awarded (0 if contract not found).
    float OnContractCompleted(uint64_t playerId,
                               const std::string& contractId,
                               float performanceMultiplier = 1.0f);

    // ---- eligibility gate -----------------------------------------------
    ContractEligibility CheckEligibility(uint64_t playerId,
                                          const std::string& contractId) const;

    // ---- standing / skill query (injected by caller) -------------------
    /// Caller sets these to provide faction standing and skill level lookups.
    using StandingQuery = std::function<float(uint64_t playerId, uint32_t factionId)>;
    using SkillQuery    = std::function<float(uint64_t playerId, const std::string& skillId)>;
    void SetStandingQuery(StandingQuery q) { m_standingQuery = std::move(q); }
    void SetSkillQuery   (SkillQuery q)    { m_skillQuery    = std::move(q); }

    // ---- callbacks -----------------------------------------------------
    void SetCreditCallback  (CreditRewardCallback  cb) { m_creditCb   = std::move(cb); }
    void SetStandingCallback(StandingRewardCallback cb) { m_standingCb = std::move(cb); }
    void SetSkillXPCallback (SkillXPCallback        cb) { m_skillCb    = std::move(cb); }

    // ---- available contracts for player (filtered by eligibility) ------
    std::vector<ContractRewardDefinition> GetAvailableContracts(
        uint64_t playerId) const;

    // ---- stats ---------------------------------------------------------
    size_t CompletedCount() const { return m_completedCount; }

private:
    std::vector<ContractRewardDefinition> m_contracts;
    size_t                                m_completedCount = 0;

    CreditRewardCallback    m_creditCb;
    StandingRewardCallback  m_standingCb;
    SkillXPCallback         m_skillCb;
    StandingQuery           m_standingQuery;
    SkillQuery              m_skillQuery;

    const ContractRewardDefinition* FindDef(const std::string& contractId) const;
};

} // namespace novaforge::integration

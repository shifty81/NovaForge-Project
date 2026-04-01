// ProgressionRewardSystem.h
// NovaForge progression — contract completion rewards: credits, faction standing,
// skill XP, and contract gating based on standing/skill level.

#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Progression
{

// --------------------------------------------------------------------------
// Reward payload
// --------------------------------------------------------------------------

struct ContractRewardPayload
{
    float    credits         = 0.f;
    float    factionStanding = 0.f; ///< delta applied to owning faction
    float    skillXP         = 0.f;
    std::string skillId;            ///< which skill receives the XP
    uint32_t factionId       = 0;   ///< faction receiving the standing change
    bool     missionProgress = false;
};

// --------------------------------------------------------------------------
// Contract gate definition
// --------------------------------------------------------------------------

struct ContractGateRequirement
{
    std::string  contractTierId;     ///< tier tag, e.g. "tier2_combat"
    uint32_t     minFactionId   = 0; ///< 0 = no faction requirement
    float        minStanding    = 0.f;
    std::string  requiredSkillId;
    uint32_t     minSkillLevel  = 0;
};

// --------------------------------------------------------------------------
// Skill progress entry — tracks both accumulated XP and computed level.
// --------------------------------------------------------------------------

struct SkillProgressEntry
{
    std::string skillId;
    uint32_t    level          = 0;
    float       accumulatedXP  = 0.f;
};

// --------------------------------------------------------------------------
// Player progression snapshot (credits + standing cache)
// --------------------------------------------------------------------------

struct PlayerProgressionRecord
{
    uint64_t playerId    = 0;
    float    credits     = 0.f;
    std::vector<std::pair<uint32_t, float>> factionStandings; ///< {factionId, standing}
    std::vector<SkillProgressEntry>         skills;
};

// --------------------------------------------------------------------------
// ProgressionRewardSystem
// --------------------------------------------------------------------------

class ProgressionRewardSystem
{
public:
    ProgressionRewardSystem()  = default;
    ~ProgressionRewardSystem() = default;

    void initialise();
    void shutdown();

    // ---- gate definitions ------------------------------------------
    void registerGate(const ContractGateRequirement& gate);

    // ---- reward application ----------------------------------------

    /// Award a contract's full reward payload to the player.
    /// Returns true if all sub-rewards were applied.
    bool applyContractReward(uint64_t playerId,
                              const ContractRewardPayload& payload);

    /// Award only credits.
    bool awardCredits(uint64_t playerId, float credits);

    /// Apply a faction standing delta (clamped to -10 / +10).
    bool applyFactionStanding(uint64_t playerId, uint32_t factionId,
                               float delta);

    /// Award skill XP (creates skill record if missing).
    bool awardSkillXP(uint64_t playerId, const std::string& skillId,
                      float xp, float xpPerLevel = 1000.f);

    // ---- gating checks ---------------------------------------------
    /// Returns true if the player meets all requirements for the contract tier.
    bool meetsGatingRequirements(uint64_t playerId,
                                  const std::string& contractTierId) const;

    // ---- queries ---------------------------------------------------
    std::optional<PlayerProgressionRecord>
        getRecord(uint64_t playerId) const;

    float  getCredits         (uint64_t playerId)                        const;
    float  getFactionStanding (uint64_t playerId, uint32_t factionId)    const;
    uint32_t getSkillLevel    (uint64_t playerId, const std::string& skillId) const;

private:
    std::vector<PlayerProgressionRecord>  records_;
    std::vector<ContractGateRequirement>  gates_;

    PlayerProgressionRecord* getMutable(uint64_t playerId);
    PlayerProgressionRecord& getOrCreate(uint64_t playerId);
};

} // namespace NovaForge::Gameplay::Progression

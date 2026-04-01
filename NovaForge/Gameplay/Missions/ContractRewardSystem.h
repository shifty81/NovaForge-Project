// ContractRewardSystem.h
// NovaForge contracts — reward resolution when a contract/mission is completed.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Missions
{

struct ContractReward
{
    float    creditsAwarded      = 0.0f;
    float    factionStandingDelta = 0.0f;
    float    skillXpAwarded      = 0.0f;
    std::string skillId;          ///< Which skill receives XP (empty = none)
    std::vector<std::string> unlockedContractTags; ///< Gate tags unlocked
};

/// Resolves and applies all rewards when a contract instance is completed.
class ContractRewardSystem
{
public:
    ContractRewardSystem()  = default;
    ~ContractRewardSystem() = default;

    void initialise();
    void shutdown();

    /// Build the reward descriptor for a given mission template.
    ContractReward buildReward(uint32_t templateId,
                               float    standingMultiplier = 1.0f) const;

    /// Apply credits to the player's wallet (stub → Economy integration).
    bool awardCredits(uint64_t playerId, float amount);

    /// Apply faction standing delta.
    bool awardFactionStanding(uint64_t playerId, uint32_t factionId, float delta);

    /// Apply skill XP.
    bool awardSkillXp(uint64_t playerId,
                      const std::string& skillId, float xp);

    /// Unlock better contracts by standing / skill level.
    void updateContractGates(uint64_t playerId,
                             const ContractReward& reward);

    /// Convenience — call buildReward then apply everything.
    bool completeContract(uint64_t playerId, uint32_t templateId,
                          uint32_t factionId);
};

} // namespace NovaForge::Gameplay::Missions

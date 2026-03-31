// ContractRewardSystem.cpp
// NovaForge contracts — reward resolution when a contract/mission is completed.

#include "Missions/ContractRewardSystem.h"

namespace NovaForge::Gameplay::Missions
{

void ContractRewardSystem::initialise() {}
void ContractRewardSystem::shutdown()   {}

ContractReward ContractRewardSystem::buildReward(uint32_t templateId,
                                                 float    standingMultiplier) const
{
    // Stub: look up MissionTemplate by templateId and scale rewards.
    ContractReward reward;
    reward.creditsAwarded        = static_cast<float>(templateId) * 100.0f;
    reward.factionStandingDelta  = 5.0f * standingMultiplier;
    reward.skillXpAwarded        = 250.0f;
    reward.skillId               = "piloting";
    return reward;
}

bool ContractRewardSystem::awardCredits(uint64_t /*playerId*/, float /*amount*/)
{
    // Stub: route into Economy/TradeMarket wallet.
    return true;
}

bool ContractRewardSystem::awardFactionStanding(uint64_t /*playerId*/,
                                                uint32_t /*factionId*/,
                                                float    /*delta*/)
{
    // Stub: update FactionRegistry standing record.
    return true;
}

bool ContractRewardSystem::awardSkillXp(uint64_t /*playerId*/,
                                        const std::string& /*skillId*/,
                                        float /*xp*/)
{
    // Stub: forward to ProgressionSystem::addXP().
    return true;
}

void ContractRewardSystem::updateContractGates(uint64_t /*playerId*/,
                                               const ContractReward& reward)
{
    // Stub: unlock higher-tier contracts when gates are met.
    for (const auto& tag : reward.unlockedContractTags) { (void)tag; }
}

bool ContractRewardSystem::completeContract(uint64_t  playerId,
                                            uint32_t  templateId,
                                            uint32_t  factionId)
{
    ContractReward reward = buildReward(templateId);
    if (!awardCredits(playerId, reward.creditsAwarded))        return false;
    if (!awardFactionStanding(playerId, factionId,
                              reward.factionStandingDelta))    return false;
    if (!reward.skillId.empty())
        awardSkillXp(playerId, reward.skillId, reward.skillXpAwarded);
    updateContractGates(playerId, reward);
    return true;
}

} // namespace NovaForge::Gameplay::Missions

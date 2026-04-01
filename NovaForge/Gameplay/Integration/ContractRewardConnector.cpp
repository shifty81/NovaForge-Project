// ContractRewardConnector.cpp
// NovaForge Integration — contract reward connector.

#include "Integration/ContractRewardConnector.h"

#include <algorithm>

namespace novaforge::integration {

bool ContractRewardConnector::Initialize()
{
    m_completedCount = 0;
    return true;
}

void ContractRewardConnector::Shutdown()
{
    m_contracts.clear();
    m_completedCount = 0;
    m_creditCb   = nullptr;
    m_standingCb = nullptr;
    m_skillCb    = nullptr;
}

void ContractRewardConnector::RegisterContract(
    const ContractRewardDefinition& def)
{
    for (auto& c : m_contracts)
        if (c.contractId == def.contractId) { c = def; return; }
    m_contracts.push_back(def);
}

bool ContractRewardConnector::UnregisterContract(
    const std::string& contractId)
{
    auto it = std::find_if(m_contracts.begin(), m_contracts.end(),
                           [&](const ContractRewardDefinition& d)
                           { return d.contractId == contractId; });
    if (it == m_contracts.end()) return false;
    m_contracts.erase(it);
    return true;
}

bool ContractRewardConnector::HasContract(
    const std::string& contractId) const
{
    return FindDef(contractId) != nullptr;
}

std::optional<ContractRewardDefinition>
ContractRewardConnector::FindContract(const std::string& contractId) const
{
    const ContractRewardDefinition* d = FindDef(contractId);
    if (d) return *d;
    return std::nullopt;
}

float ContractRewardConnector::OnContractCompleted(
    uint64_t playerId,
    const std::string& contractId,
    float performanceMultiplier)
{
    const ContractRewardDefinition* def = FindDef(contractId);
    if (!def) return 0.f;

    float credits = def->baseCreditReward * performanceMultiplier;

    // Award credits.
    if (m_creditCb && credits > 0.f)
        m_creditCb(playerId, credits, "Contract: " + contractId);

    // Award faction standing.
    if (def->factionStandingDelta != 0.f && def->primaryFactionId != 0)
    {
        float current = 0.f;
        if (m_standingQuery)
            current = m_standingQuery(playerId, def->primaryFactionId);

        StandingChangeEvent evt;
        evt.playerId    = playerId;
        evt.factionId   = def->primaryFactionId;
        evt.delta       = def->factionStandingDelta * performanceMultiplier;
        evt.newStanding = current + evt.delta;
        if (m_standingCb) m_standingCb(evt);
    }

    // Award skill XP.
    if (def->skillXPAmount > 0.f && !def->rewardSkillId.empty())
    {
        SkillXPEvent xpEvt;
        xpEvt.playerId  = playerId;
        xpEvt.skillId   = def->rewardSkillId;
        xpEvt.xpGained  = def->skillXPAmount * performanceMultiplier;
        if (m_skillCb) m_skillCb(xpEvt);
    }

    ++m_completedCount;
    return credits;
}

ContractEligibility ContractRewardConnector::CheckEligibility(
    uint64_t playerId, const std::string& contractId) const
{
    ContractEligibility result;
    const ContractRewardDefinition* def = FindDef(contractId);
    if (!def)
    {
        result.eligible   = false;
        result.failReason = "Contract not found";
        return result;
    }

    // Check faction standing requirement.
    if (def->requiredStanding > -10.f && def->primaryFactionId != 0)
    {
        float standing = 0.f;
        if (m_standingQuery)
            standing = m_standingQuery(playerId, def->primaryFactionId);
        result.playerStanding = standing;
        if (standing < def->requiredStanding)
        {
            result.eligible   = false;
            result.failReason = "Insufficient faction standing ("
                                + std::to_string(standing)
                                + " < " + std::to_string(def->requiredStanding) + ")";
            return result;
        }
    }

    // Check skill level requirement.
    if (def->requiredSkillLevel > 0.f && !def->requiredSkillId.empty())
    {
        float level = 0.f;
        if (m_skillQuery)
            level = m_skillQuery(playerId, def->requiredSkillId);
        result.playerSkillLevel = level;
        if (level < def->requiredSkillLevel)
        {
            result.eligible   = false;
            result.failReason = "Insufficient skill level ("
                                + std::to_string(level)
                                + " < " + std::to_string(def->requiredSkillLevel) + ")";
            return result;
        }
    }

    result.eligible = true;
    return result;
}

std::vector<ContractRewardDefinition>
ContractRewardConnector::GetAvailableContracts(uint64_t playerId) const
{
    std::vector<ContractRewardDefinition> result;
    for (const auto& def : m_contracts)
    {
        auto elig = CheckEligibility(playerId, def.contractId);
        if (elig.eligible) result.push_back(def);
    }
    return result;
}

const ContractRewardDefinition* ContractRewardConnector::FindDef(
    const std::string& contractId) const
{
    for (const auto& c : m_contracts)
        if (c.contractId == contractId) return &c;
    return nullptr;
}

} // namespace novaforge::integration

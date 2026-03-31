// ProgressionRewardSystem.cpp
// NovaForge progression — contract rewards, faction standing, skill XP, gating.

#include "Progression/ProgressionRewardSystem.h"

#include <algorithm>
#include <cmath>

namespace NovaForge::Gameplay::Progression
{

void ProgressionRewardSystem::initialise() {}
void ProgressionRewardSystem::shutdown()
{
    records_.clear();
    gates_.clear();
}

void ProgressionRewardSystem::registerGate(const ContractGateRequirement& gate)
{
    gates_.push_back(gate);
}

bool ProgressionRewardSystem::applyContractReward(
    uint64_t playerId, const ContractRewardPayload& payload)
{
    bool ok = true;
    if (payload.credits > 0.f)
        ok &= awardCredits(playerId, payload.credits);
    if (payload.factionId != 0 && payload.factionStanding != 0.f)
        ok &= applyFactionStanding(playerId, payload.factionId,
                                   payload.factionStanding);
    if (!payload.skillId.empty() && payload.skillXP > 0.f)
        ok &= awardSkillXP(playerId, payload.skillId, payload.skillXP);
    return ok;
}

bool ProgressionRewardSystem::awardCredits(uint64_t playerId, float credits)
{
    if (credits <= 0.f) return false;
    getOrCreate(playerId).credits += credits;
    return true;
}

bool ProgressionRewardSystem::applyFactionStanding(uint64_t playerId,
                                                    uint32_t factionId,
                                                    float delta)
{
    auto& rec = getOrCreate(playerId);
    for (auto& [fid, standing] : rec.factionStandings)
    {
        if (fid == factionId)
        {
            standing = std::max(-10.f, std::min(10.f, standing + delta));
            return true;
        }
    }
    float clamped = std::max(-10.f, std::min(10.f, delta));
    rec.factionStandings.emplace_back(factionId, clamped);
    return true;
}

bool ProgressionRewardSystem::awardSkillXP(uint64_t playerId,
                                             const std::string& skillId,
                                             float xp,
                                             float xpPerLevel)
{
    if (xp <= 0.f || skillId.empty()) return false;

    auto& rec = getOrCreate(playerId);

    // Find existing skill entry and accumulate XP.
    for (auto& entry : rec.skills)
    {
        if (entry.skillId == skillId)
        {
            entry.accumulatedXP += xp;
            entry.level = std::min(
                static_cast<uint32_t>(entry.accumulatedXP / xpPerLevel),
                10u);
            return true;
        }
    }

    // New skill entry.
    SkillProgressEntry entry;
    entry.skillId       = skillId;
    entry.accumulatedXP = xp;
    entry.level         = std::min(
        static_cast<uint32_t>(xp / xpPerLevel), 10u);
    rec.skills.push_back(entry);
    return true;
}

bool ProgressionRewardSystem::meetsGatingRequirements(
    uint64_t playerId, const std::string& contractTierId) const
{
    for (const auto& gate : gates_)
    {
        if (gate.contractTierId != contractTierId) continue;

        // Check faction standing.
        if (gate.minFactionId != 0)
        {
            if (getFactionStanding(playerId, gate.minFactionId) < gate.minStanding)
                return false;
        }

        // Check skill level.
        if (!gate.requiredSkillId.empty())
        {
            if (getSkillLevel(playerId, gate.requiredSkillId) < gate.minSkillLevel)
                return false;
        }
    }
    return true;
}

std::optional<PlayerProgressionRecord>
ProgressionRewardSystem::getRecord(uint64_t playerId) const
{
    for (const auto& r : records_)
        if (r.playerId == playerId) return r;
    return std::nullopt;
}

float ProgressionRewardSystem::getCredits(uint64_t playerId) const
{
    for (const auto& r : records_)
        if (r.playerId == playerId) return r.credits;
    return 0.f;
}

float ProgressionRewardSystem::getFactionStanding(uint64_t playerId,
                                                   uint32_t factionId) const
{
    for (const auto& r : records_)
    {
        if (r.playerId != playerId) continue;
        for (const auto& [fid, standing] : r.factionStandings)
            if (fid == factionId) return standing;
    }
    return 0.f;
}

uint32_t ProgressionRewardSystem::getSkillLevel(uint64_t playerId,
                                                  const std::string& skillId) const
{
    for (const auto& r : records_)
    {
        if (r.playerId != playerId) continue;
        for (const auto& entry : r.skills)
            if (entry.skillId == skillId) return entry.level;
    }
    return 0;
}

PlayerProgressionRecord* ProgressionRewardSystem::getMutable(uint64_t playerId)
{
    for (auto& r : records_)
        if (r.playerId == playerId) return &r;
    return nullptr;
}

PlayerProgressionRecord& ProgressionRewardSystem::getOrCreate(uint64_t playerId)
{
    PlayerProgressionRecord* p = getMutable(playerId);
    if (p) return *p;
    records_.push_back({ playerId, 0.f, {}, {} });
    return records_.back();
}

} // namespace NovaForge::Gameplay::Progression

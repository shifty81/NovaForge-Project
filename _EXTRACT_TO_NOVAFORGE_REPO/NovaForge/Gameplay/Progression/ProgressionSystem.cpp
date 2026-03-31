// ProgressionSystem.cpp
#include "ProgressionSystem.h"

namespace NovaForge::Gameplay::Progression
{

void ProgressionSystem::initialise() {}
void ProgressionSystem::shutdown()   {}

void ProgressionSystem::registerSkill(const SkillDefinition& def)
{
    skillDefs_[def.id] = def;
}

bool ProgressionSystem::awardXP(uint64_t entityId, const std::string& skillId, float xp)
{
    auto it = skillDefs_.find(skillId);
    if (it == skillDefs_.end()) return false;
    const SkillDefinition& def = it->second;

    auto& vec = progress_[entityId];
    for (auto& sp : vec)
    {
        if (sp.skillId != skillId) continue;
        sp.currentXP += xp;
        while (sp.currentLevel < def.maxLevel && sp.currentXP >= def.xpPerLevel)
        {
            sp.currentXP -= def.xpPerLevel;
            ++sp.currentLevel;
        }
        return true;
    }
    // First time this entity trains this skill
    SkillProgress sp;
    sp.skillId    = skillId;
    sp.currentXP  = xp;
    const float xpp = def.xpPerLevel > 0.0f ? def.xpPerLevel : 1.0f;
    while (sp.currentLevel < def.maxLevel && sp.currentXP >= xpp)
    {
        sp.currentXP -= xpp;
        ++sp.currentLevel;
    }
    vec.push_back(sp);
    return true;
}

std::optional<SkillProgress> ProgressionSystem::getProgress(
    uint64_t entityId, const std::string& skillId) const
{
    auto it = progress_.find(entityId);
    if (it == progress_.end()) return std::nullopt;
    for (const auto& sp : it->second)
        if (sp.skillId == skillId) return sp;
    return std::nullopt;
}

std::vector<SkillProgress> ProgressionSystem::listSkills(uint64_t entityId) const
{
    auto it = progress_.find(entityId);
    if (it == progress_.end()) return {};
    return it->second;
}

bool ProgressionSystem::hasSkillLevel(
    uint64_t entityId, const std::string& skillId, uint32_t level) const
{
    auto prog = getProgress(entityId, skillId);
    return prog.has_value() && prog->currentLevel >= level;
}

} // namespace NovaForge::Gameplay::Progression

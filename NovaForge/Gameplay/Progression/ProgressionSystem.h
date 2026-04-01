// ProgressionSystem.h
// NovaForge player/entity progression — skills, experience, levels, unlocks.

#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

namespace NovaForge::Gameplay::Progression
{

struct SkillDefinition
{
    std::string id;
    std::string displayName;
    uint32_t    maxLevel    = 5;
    float       xpPerLevel  = 1000.0f;
};

struct SkillProgress
{
    std::string skillId;
    uint32_t    currentLevel = 0;
    float       currentXP    = 0.0f;
};

class ProgressionSystem
{
public:
    ProgressionSystem()  = default;
    ~ProgressionSystem() = default;

    void initialise();
    void shutdown();

    void registerSkill(const SkillDefinition& def);
    bool awardXP(uint64_t entityId, const std::string& skillId, float xp);
    std::optional<SkillProgress> getProgress(uint64_t entityId, const std::string& skillId) const;
    std::vector<SkillProgress>   listSkills(uint64_t entityId) const;
    bool hasSkillLevel(uint64_t entityId, const std::string& skillId, uint32_t level) const;

private:
    std::unordered_map<std::string, SkillDefinition> skillDefs_;
    std::unordered_map<uint64_t, std::vector<SkillProgress>> progress_;
};

} // namespace NovaForge::Gameplay::Progression

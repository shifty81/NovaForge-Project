#pragma once

#include <string>
#include <vector>

struct SkillProgress
{
    std::string SkillId;
    int Level = 1;
    float XP = 0.0f;
    float TrainingMultiplier = 1.0f;
};

struct ProgressionState
{
    std::vector<SkillProgress> Skills;
};

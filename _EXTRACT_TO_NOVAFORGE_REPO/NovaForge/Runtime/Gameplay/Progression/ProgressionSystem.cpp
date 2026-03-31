#include "Gameplay/Progression/ProgressionSystem.h"
#include <iostream>

bool ProgressionSystem::Initialize()
{
    std::cout << "[Progression] Initialize\n";
    return true;
}

void ProgressionSystem::RegisterSkill(const std::string& SkillId)
{
    for (const auto& Skill : State.Skills)
    {
        if (Skill.SkillId == SkillId)
        {
            return;
        }
    }

    State.Skills.push_back({SkillId, 1, 0.0f, 1.0f});
    std::cout << "[Progression] Registered skill " << SkillId << "\n";
}

void ProgressionSystem::AwardXP(const std::string& SkillId, float Amount)
{
    for (auto& Skill : State.Skills)
    {
        if (Skill.SkillId == SkillId)
        {
            Skill.XP += Amount * Skill.TrainingMultiplier;

            while (Skill.XP >= 100.0f)
            {
                Skill.XP -= 100.0f;
                Skill.Level += 1;
                std::cout << "[Progression] " << Skill.SkillId << " leveled to " << Skill.Level << "\n";
            }

            std::cout << "[Progression] Awarded XP to " << Skill.SkillId
                      << " XP=" << Skill.XP
                      << " Mult=" << Skill.TrainingMultiplier << "\n";
            return;
        }
    }
}

void ProgressionSystem::ApplyTrainingAcceleration(const std::string& SkillId, float MultiplierBonus)
{
    for (auto& Skill : State.Skills)
    {
        if (Skill.SkillId == SkillId)
        {
            Skill.TrainingMultiplier += MultiplierBonus;
            std::cout << "[Progression] Training acceleration for " << Skill.SkillId
                      << " -> " << Skill.TrainingMultiplier << "\n";
            return;
        }
    }
}

const SkillProgress* ProgressionSystem::FindSkill(const std::string& SkillId) const
{
    for (const auto& Skill : State.Skills)
    {
        if (Skill.SkillId == SkillId)
        {
            return &Skill;
        }
    }

    return nullptr;
}

void ProgressionSystem::Tick(float)
{
    std::cout << "[Progression] Skills=" << State.Skills.size() << "\n";
}

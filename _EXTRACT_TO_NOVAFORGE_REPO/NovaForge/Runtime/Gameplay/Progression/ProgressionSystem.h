#pragma once

#include "Gameplay/Progression/ProgressionTypes.h"
#include <string>

class ProgressionSystem
{
public:
    bool Initialize();
    void RegisterSkill(const std::string& SkillId);
    void AwardXP(const std::string& SkillId, float Amount);
    void ApplyTrainingAcceleration(const std::string& SkillId, float MultiplierBonus);
    const SkillProgress* FindSkill(const std::string& SkillId) const;
    void Tick(float DeltaTime);

private:
    ProgressionState State;
};

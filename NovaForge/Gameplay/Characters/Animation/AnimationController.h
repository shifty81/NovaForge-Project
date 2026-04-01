#pragma once

#include "Characters/Animation/AnimationTypes.h"
#include "Characters/CharacterTypes.h"
#include <string>
#include <vector>

class AnimationController
{
public:
    bool Initialize();
    void RegisterCharacter(const std::string& CharacterId);
    void UpdateMovementState(const std::string& CharacterId, ECharacterMovementMode MovementMode, float Speed, bool bBoosting);
    const AnimationState* FindState(const std::string& CharacterId) const;
    void Tick(float DeltaTime);

private:
    std::vector<AnimationState> States;
};

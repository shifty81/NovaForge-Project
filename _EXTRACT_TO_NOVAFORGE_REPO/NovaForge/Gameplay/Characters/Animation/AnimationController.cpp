#include "Characters/Animation/AnimationController.h"
#include <iostream>

bool AnimationController::Initialize()
{
    std::cout << "[AnimationController] Initialize\n";
    return true;
}

void AnimationController::RegisterCharacter(const std::string& CharacterId)
{
    States.push_back({CharacterId, ECharacterAnimState::Idle, 0.0f});
    std::cout << "[AnimationController] Registered " << CharacterId << "\n";
}

void AnimationController::UpdateMovementState(const std::string& CharacterId, ECharacterMovementMode MovementMode, float Speed, bool bBoosting)
{
    for (auto& Entry : States)
    {
        if (Entry.CharacterId == CharacterId)
        {
            Entry.Speed = Speed;

            switch (MovementMode)
            {
                case ECharacterMovementMode::FPS:
                    Entry.State = (Speed > 10.0f) ? ECharacterAnimState::Walk : ECharacterAnimState::Idle;
                    break;
                case ECharacterMovementMode::EVA:
                    Entry.State = bBoosting ? ECharacterAnimState::EVABoost : ECharacterAnimState::EVAFloat;
                    break;
                case ECharacterMovementMode::Mech:
                    Entry.State = (Speed > 10.0f) ? ECharacterAnimState::MechWalk : ECharacterAnimState::MechIdle;
                    break;
            }

            std::cout << "[AnimationController] Updated state for " << CharacterId
                      << " -> " << static_cast<int>(Entry.State) << "\n";
            return;
        }
    }
}

const AnimationState* AnimationController::FindState(const std::string& CharacterId) const
{
    for (const auto& Entry : States)
    {
        if (Entry.CharacterId == CharacterId)
        {
            return &Entry;
        }
    }
    return nullptr;
}

void AnimationController::Tick(float)
{
    std::cout << "[AnimationController] States=" << States.size() << "\n";
}

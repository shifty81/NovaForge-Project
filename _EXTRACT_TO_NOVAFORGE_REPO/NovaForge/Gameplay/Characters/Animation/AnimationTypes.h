#pragma once

#include <string>

enum class ECharacterAnimState
{
    Idle,
    Walk,
    Run,
    EVAFloat,
    EVABoost,
    Interact,
    MechIdle,
    MechWalk
};

struct AnimationState
{
    std::string CharacterId;
    ECharacterAnimState State = ECharacterAnimState::Idle;
    float Speed = 0.0f;
};

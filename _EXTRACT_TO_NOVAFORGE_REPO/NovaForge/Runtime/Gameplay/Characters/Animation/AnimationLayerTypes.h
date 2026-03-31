#pragma once

#include <string>

enum class EAnimationLayerType
{
    BaseLocomotion,
    AdditiveBreathing,
    UpperBodyTool,
    Fingers,
    ProceduralOffsets
};

struct AnimationLayerState
{
    std::string CharacterId;
    EAnimationLayerType Layer = EAnimationLayerType::BaseLocomotion;
    std::string ClipId;
    float Weight = 1.0f;
    bool bEnabled = true;
};

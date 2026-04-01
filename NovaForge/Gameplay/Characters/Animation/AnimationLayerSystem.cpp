#include "Characters/Animation/AnimationLayerSystem.h"
#include <iostream>

bool AnimationLayerSystem::Initialize()
{
    std::cout << "[AnimationLayerSystem] Initialize\n";
    return true;
}

void AnimationLayerSystem::RegisterCharacter(const std::string& CharacterId)
{
    Layers.push_back({CharacterId, EAnimationLayerType::BaseLocomotion, "anim_base_idle", 1.0f, true});
    Layers.push_back({CharacterId, EAnimationLayerType::AdditiveBreathing, "anim_add_breathing", 0.35f, true});
    Layers.push_back({CharacterId, EAnimationLayerType::UpperBodyTool, "anim_tool_idle", 1.0f, true});
    Layers.push_back({CharacterId, EAnimationLayerType::Fingers, "anim_fingers_relaxed", 1.0f, true});
    Layers.push_back({CharacterId, EAnimationLayerType::ProceduralOffsets, "proc_offsets", 1.0f, true});
}

void AnimationLayerSystem::EvaluateFromCharacterState(const AuthoritativeCharacterState& State)
{
    for (auto& Layer : Layers)
    {
        if (Layer.CharacterId != State.CharacterId)
        {
            continue;
        }

        if (Layer.Layer == EAnimationLayerType::BaseLocomotion)
        {
            if (State.MovementMode == ECharacterMovementMode::FPS)
            {
                Layer.ClipId = (State.Intent.Forward != 0.0f || State.Intent.Right != 0.0f) ? "anim_fps_walk" : "anim_fps_idle";
            }
            else if (State.MovementMode == ECharacterMovementMode::EVA)
            {
                Layer.ClipId = State.Intent.bBoost ? "anim_eva_boost" : "anim_eva_float";
            }
            else
            {
                Layer.ClipId = "anim_mech_idle";
            }
        }

        if (Layer.Layer == EAnimationLayerType::Fingers)
        {
            Layer.ClipId = State.Intent.bInteract ? "anim_fingers_press" : "anim_fingers_relaxed";
        }
    }
}

const std::vector<AnimationLayerState>& AnimationLayerSystem::GetLayers() const
{
    return Layers;
}

void AnimationLayerSystem::Tick(float)
{
    std::cout << "[AnimationLayerSystem] Layers=" << Layers.size() << "\n";
}

#include "Gameplay/Characters/IK/IKSystem.h"
#include <iostream>

bool IKSystem::Initialize()
{
    std::cout << "[IKSystem] Initialize\n";
    return true;
}

void IKSystem::RegisterCharacter(const std::string& CharacterId)
{
    Targets.push_back({CharacterId, EIKTargetType::HandRight, 20.0f, 5.0f, 0.0f, true});
    Targets.push_back({CharacterId, EIKTargetType::HandLeft, 18.0f, -5.0f, 0.0f, true});
    Targets.push_back({CharacterId, EIKTargetType::FootRight, 0.0f, 6.0f, -90.0f, true});
    Targets.push_back({CharacterId, EIKTargetType::FootLeft, 0.0f, -6.0f, -90.0f, true});
    Targets.push_back({CharacterId, EIKTargetType::LookTarget, 100.0f, 0.0f, 160.0f, true});
}

void IKSystem::EvaluateFromCharacterState(const AuthoritativeCharacterState& State)
{
    for (auto& Target : Targets)
    {
        if (Target.CharacterId != State.CharacterId)
        {
            continue;
        }

        if (Target.Type == EIKTargetType::LookTarget)
        {
            Target.X = State.PositionX + 100.0f;
            Target.Y = State.PositionY + State.Intent.LookYaw;
            Target.Z = State.PositionZ + 160.0f + State.Intent.LookPitch;
        }
    }
}

const std::vector<IKTarget>& IKSystem::GetTargets() const
{
    return Targets;
}

void IKSystem::Tick(float)
{
    std::cout << "[IKSystem] Targets=" << Targets.size() << "\n";
}

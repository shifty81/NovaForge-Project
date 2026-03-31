#include "Gameplay/Characters/FPS/FPSPresentationSystem.h"
#include <iostream>

bool FPSPresentationSystem::Initialize()
{
    std::cout << "[FPSPresentationSystem] Initialize\n";
    return true;
}

void FPSPresentationSystem::RegisterCharacter(const std::string& CharacterId)
{
    States.push_back({CharacterId, true, true, true, 0.0f, 0.0f, 168.0f, 0.35f});
}

void FPSPresentationSystem::EvaluateFromCharacterState(const AuthoritativeCharacterState& State)
{
    for (auto& Entry : States)
    {
        if (Entry.CharacterId == State.CharacterId)
        {
            Entry.CameraOffsetZ = 168.0f;
            if (State.MovementMode == ECharacterMovementMode::EVA)
            {
                Entry.HandIdleSwayWeight = 0.15f;
            }
            else
            {
                Entry.HandIdleSwayWeight = 0.35f;
            }
        }
    }
}

const FPSPresentationState* FPSPresentationSystem::FindState(const std::string& CharacterId) const
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

void FPSPresentationSystem::Tick(float)
{
    std::cout << "[FPSPresentationSystem] States=" << States.size() << "\n";
}

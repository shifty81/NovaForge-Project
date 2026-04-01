#include "Characters/Core/CharacterStateAuthority.h"
#include <iostream>

bool CharacterStateAuthority::Initialize()
{
    std::cout << "[CharacterStateAuthority] Initialize\n";
    return true;
}

void CharacterStateAuthority::RegisterCharacter(const AuthoritativeCharacterState& State)
{
    States.push_back(State);
    std::cout << "[CharacterStateAuthority] Registered " << State.CharacterId << "\n";
}

AuthoritativeCharacterState* CharacterStateAuthority::FindMutable(const std::string& CharacterId)
{
    for (auto& State : States)
    {
        if (State.CharacterId == CharacterId)
        {
            return &State;
        }
    }
    return nullptr;
}

const AuthoritativeCharacterState* CharacterStateAuthority::Find(const std::string& CharacterId) const
{
    for (const auto& State : States)
    {
        if (State.CharacterId == CharacterId)
        {
            return &State;
        }
    }
    return nullptr;
}

void CharacterStateAuthority::ApplyIntent(const std::string& CharacterId, const MovementIntent& Intent)
{
    if (auto* State = FindMutable(CharacterId))
    {
        State->Intent = Intent;
    }
}

void CharacterStateAuthority::Tick(float DeltaTime)
{
    for (auto& State : States)
    {
        State.VelocityX = State.Intent.Forward * 300.0f;
        State.VelocityY = State.Intent.Right * 300.0f;
        State.VelocityZ = State.Intent.Up * 300.0f;

        State.PositionX += State.VelocityX * DeltaTime;
        State.PositionY += State.VelocityY * DeltaTime;
        State.PositionZ += State.VelocityZ * DeltaTime;

        std::cout << "[CharacterStateAuthority] " << State.CharacterId
                  << " Pos=(" << State.PositionX << ", " << State.PositionY << ", " << State.PositionZ << ")"
                  << " Mode=" << static_cast<int>(State.MovementMode) << "\n";
    }
}

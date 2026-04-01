#include "Gameplay/Characters/CharacterSystem.h"
#include <iostream>

bool CharacterSystem::Initialize()
{
    std::cout << "[CharacterSystem] Initialize\n";
    return true;
}

void CharacterSystem::RegisterCharacter(const CharacterRigState& Character)
{
    Characters.push_back(Character);
    std::cout << "[CharacterSystem] Registered " << Character.CharacterId << "\n";
}

CharacterRigState* CharacterSystem::FindCharacterMutable(const std::string& CharacterId)
{
    for (auto& Character : Characters)
    {
        if (Character.CharacterId == CharacterId)
        {
            return &Character;
        }
    }
    return nullptr;
}

const CharacterRigState* CharacterSystem::FindCharacter(const std::string& CharacterId) const
{
    for (const auto& Character : Characters)
    {
        if (Character.CharacterId == CharacterId)
        {
            return &Character;
        }
    }
    return nullptr;
}

void CharacterSystem::SetMovementMode(const std::string& CharacterId, ECharacterMovementMode Mode)
{
    if (auto* Character = FindCharacterMutable(CharacterId))
    {
        Character->MovementMode = Mode;
        std::cout << "[CharacterSystem] Movement mode changed for "
                  << CharacterId << " -> " << static_cast<int>(Mode) << "\n";
    }
}

void CharacterSystem::Tick(float)
{
    std::cout << "[CharacterSystem] Characters=" << Characters.size() << "\n";
}

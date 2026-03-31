#pragma once

#include "Gameplay/Characters/Core/CharacterStateTypes.h"
#include <string>
#include <vector>

class CharacterStateAuthority
{
public:
    bool Initialize();
    void RegisterCharacter(const AuthoritativeCharacterState& State);
    AuthoritativeCharacterState* FindMutable(const std::string& CharacterId);
    const AuthoritativeCharacterState* Find(const std::string& CharacterId) const;
    void ApplyIntent(const std::string& CharacterId, const MovementIntent& Intent);
    void Tick(float DeltaTime);

private:
    std::vector<AuthoritativeCharacterState> States;
};

#pragma once

#include "Characters/CharacterTypes.h"
#include <string>
#include <vector>

class CharacterSystem
{
public:
    bool Initialize();
    void RegisterCharacter(const CharacterRigState& Character);
    CharacterRigState* FindCharacterMutable(const std::string& CharacterId);
    const CharacterRigState* FindCharacter(const std::string& CharacterId) const;
    void SetMovementMode(const std::string& CharacterId, ECharacterMovementMode Mode);
    void Tick(float DeltaTime);

private:
    std::vector<CharacterRigState> Characters;
};

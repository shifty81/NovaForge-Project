#pragma once

#include "Gameplay/Characters/IK/IKTypes.h"
#include "Gameplay/Characters/Core/CharacterStateTypes.h"
#include <string>
#include <vector>

class IKSystem
{
public:
    bool Initialize();
    void RegisterCharacter(const std::string& CharacterId);
    void EvaluateFromCharacterState(const AuthoritativeCharacterState& State);
    const std::vector<IKTarget>& GetTargets() const;
    void Tick(float DeltaTime);

private:
    std::vector<IKTarget> Targets;
};

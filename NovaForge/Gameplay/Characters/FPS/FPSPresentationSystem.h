#pragma once

#include "Characters/FPS/FPSPresentationTypes.h"
#include "Characters/Core/CharacterStateTypes.h"
#include <string>
#include <vector>

class FPSPresentationSystem
{
public:
    bool Initialize();
    void RegisterCharacter(const std::string& CharacterId);
    void EvaluateFromCharacterState(const AuthoritativeCharacterState& State);
    const FPSPresentationState* FindState(const std::string& CharacterId) const;
    void Tick(float DeltaTime);

private:
    std::vector<FPSPresentationState> States;
};

#pragma once

#include "Characters/Animation/AnimationLayerTypes.h"
#include "Characters/Core/CharacterStateTypes.h"
#include <string>
#include <vector>

class AnimationLayerSystem
{
public:
    bool Initialize();
    void RegisterCharacter(const std::string& CharacterId);
    void EvaluateFromCharacterState(const AuthoritativeCharacterState& State);
    const std::vector<AnimationLayerState>& GetLayers() const;
    void Tick(float DeltaTime);

private:
    std::vector<AnimationLayerState> Layers;
};

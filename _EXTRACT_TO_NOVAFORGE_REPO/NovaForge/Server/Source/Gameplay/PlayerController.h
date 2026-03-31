#pragma once

#include "Gameplay/GameplayTypes.h"
#include <string>

class PlayerController
{
public:
    bool Initialize(const std::string& InPlayerId, const std::string& InDisplayName);
    void Move(float ForwardAmount, float RightAmount, float UpAmount);
    void Look(float DeltaYaw, float DeltaPitch);
    void Tick(float DeltaTime);

    const PlayerComponent& GetPlayerComponent() const;
    const TransformComponent& GetTransform() const;

private:
    PlayerComponent Player;
    TransformComponent Transform;
};

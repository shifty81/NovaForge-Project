#include "Gameplay/PlayerController.h"
#include <iostream>

bool PlayerController::Initialize(const std::string& InPlayerId, const std::string& InDisplayName)
{
    Player.PlayerId = InPlayerId;
    Player.DisplayName = InDisplayName;
    Player.bIsLocalPlayer = true;

    std::cout << "[PlayerController] Initialized PlayerId=" << Player.PlayerId
              << " Name=" << Player.DisplayName << "\n";
    return true;
}

void PlayerController::Move(float ForwardAmount, float RightAmount, float UpAmount)
{
    Transform.Position.X += ForwardAmount;
    Transform.Position.Y += RightAmount;
    Transform.Position.Z += UpAmount;

    std::cout << "[PlayerController] Move -> Pos("
              << Transform.Position.X << ", "
              << Transform.Position.Y << ", "
              << Transform.Position.Z << ")\n";
}

void PlayerController::Look(float DeltaYaw, float DeltaPitch)
{
    Transform.Yaw += DeltaYaw;
    Transform.Pitch += DeltaPitch;

    std::cout << "[PlayerController] Look -> Yaw=" << Transform.Yaw
              << " Pitch=" << Transform.Pitch << "\n";
}

void PlayerController::Tick(float)
{
}

const PlayerComponent& PlayerController::GetPlayerComponent() const
{
    return Player;
}

const TransformComponent& PlayerController::GetTransform() const
{
    return Transform;
}

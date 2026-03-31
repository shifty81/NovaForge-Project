#include "Gameplay/EVA/EVAMovementController.h"
#include <iostream>

bool EVAMovementController::Initialize()
{
    std::cout << "[EVAMovement] Initialize\n";
    return true;
}

void EVAMovementController::SetActive(bool bInActive)
{
    State.bActive = bInActive;
    std::cout << "[EVAMovement] Active=" << (State.bActive ? "true" : "false") << "\n";
}

void EVAMovementController::SetThrust(float Forward, float Right, float Up)
{
    State.ThrustForward = Forward;
    State.ThrustRight = Right;
    State.ThrustUp = Up;
}

void EVAMovementController::ToggleDampening()
{
    State.bDampeningEnabled = !State.bDampeningEnabled;
    std::cout << "[EVAMovement] Dampening=" << (State.bDampeningEnabled ? "Enabled" : "Disabled") << "\n";
}

void EVAMovementController::Tick(float DeltaTime)
{
    if (!State.bActive)
    {
        return;
    }

    State.VelocityX += State.ThrustForward * DeltaTime;
    State.VelocityY += State.ThrustRight * DeltaTime;
    State.VelocityZ += State.ThrustUp * DeltaTime;

    if (State.bDampeningEnabled)
    {
        State.VelocityX *= 0.92f;
        State.VelocityY *= 0.92f;
        State.VelocityZ *= 0.92f;
    }

    std::cout << "[EVAMovement] Velocity=("
              << State.VelocityX << ", "
              << State.VelocityY << ", "
              << State.VelocityZ << ")\n";
}

const EVAState& EVAMovementController::GetState() const
{
    return State;
}

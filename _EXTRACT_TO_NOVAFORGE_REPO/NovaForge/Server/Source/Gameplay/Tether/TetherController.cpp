#include "Gameplay/Tether/TetherController.h"
#include <iostream>

bool TetherController::Initialize(float InMaxLength)
{
    State.MaxLength = InMaxLength;
    std::cout << "[Tether] Initialize MaxLength=" << State.MaxLength << "\n";
    return true;
}

void TetherController::Activate()
{
    State.bActive = true;
    State.bSupplyingOxygen = true;
    State.bSupplyingPower = true;
    std::cout << "[Tether] Activated\n";
}

void TetherController::Deactivate()
{
    State.bActive = false;
    State.bSupplyingOxygen = false;
    State.bSupplyingPower = false;
    std::cout << "[Tether] Deactivated\n";
}

void TetherController::SetLength(float NewLength)
{
    if (NewLength < 0.0f) NewLength = 0.0f;
    if (NewLength > State.MaxLength) NewLength = State.MaxLength;
    State.CurrentLength = NewLength;
}

void TetherController::Tick(float)
{
    std::cout << "[Tether] Active=" << (State.bActive ? "Yes" : "No")
              << " Length=" << State.CurrentLength
              << "/" << State.MaxLength
              << " O2=" << (State.bSupplyingOxygen ? "Yes" : "No")
              << " Power=" << (State.bSupplyingPower ? "Yes" : "No")
              << "\n";
}

const TetherState& TetherController::GetState() const
{
    return State;
}

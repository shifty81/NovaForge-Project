#include "Gameplay/Environment/SurvivalController.h"
#include "Gameplay/Tether/TetherController.h"
#include <iostream>

bool SurvivalController::Initialize()
{
    std::cout << "[Survival] Initialize\n";
    return true;
}

void SurvivalController::SetInPressurizedInterior(bool bInterior)
{
    State.bInPressurizedInterior = bInterior;
}

void SurvivalController::Tick(float DeltaTime, const TetherController& Tether)
{
    const auto& TetherState = Tether.GetState();

    State.bReceivingTetherOxygen = TetherState.bSupplyingOxygen;
    State.bReceivingTetherPower = TetherState.bSupplyingPower;

    if (!State.bInPressurizedInterior)
    {
        if (!State.bReceivingTetherOxygen)
        {
            State.SuitOxygen -= 5.0f * DeltaTime;
        }
        if (!State.bReceivingTetherPower)
        {
            State.SuitPower -= 2.5f * DeltaTime;
        }
    }

    if (State.bInPressurizedInterior)
    {
        State.SuitOxygen = 100.0f;
    }

    if (State.SuitOxygen < 0.0f) State.SuitOxygen = 0.0f;
    if (State.SuitPower < 0.0f) State.SuitPower = 0.0f;

    std::cout << "[Survival] Oxygen=" << State.SuitOxygen
              << " Power=" << State.SuitPower
              << " Interior=" << (State.bInPressurizedInterior ? "Yes" : "No")
              << "\n";
}

const SurvivalState& SurvivalController::GetState() const
{
    return State;
}

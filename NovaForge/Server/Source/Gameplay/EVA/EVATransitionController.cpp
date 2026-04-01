#include "Gameplay/EVA/EVATransitionController.h"
#include "Gameplay/Airlock/AirlockController.h"
#include "Gameplay/EVA/EVAMovementController.h"
#include "Gameplay/Tether/TetherController.h"
#include "Gameplay/Environment/SurvivalController.h"
#include <iostream>

bool EVATransitionController::Initialize(AirlockController& InAirlock, EVAMovementController& InMovement, TetherController& InTether, SurvivalController& InSurvival)
{
    Airlock = &InAirlock;
    Movement = &InMovement;
    Tether = &InTether;
    Survival = &InSurvival;
    return true;
}

bool EVATransitionController::TryExitShip()
{
    if (!Airlock->CanExitToEVA())
    {
        std::cout << "[EVATransition] Cannot exit ship yet\n";
        return false;
    }

    Mode = EPlayerMode::EVA;
    Movement->SetActive(true);
    Tether->Activate();
    Survival->SetInPressurizedInterior(false);

    std::cout << "[EVATransition] Entered EVA mode\n";
    return true;
}

bool EVATransitionController::TryReturnToShip()
{
    if (!Airlock->CanReturnToInterior())
    {
        std::cout << "[EVATransition] Cannot return to interior yet\n";
        return false;
    }

    Mode = EPlayerMode::Interior;
    Movement->SetActive(false);
    Tether->Deactivate();
    Survival->SetInPressurizedInterior(true);

    std::cout << "[EVATransition] Returned to interior mode\n";
    return true;
}

void EVATransitionController::Tick(float)
{
    std::cout << "[EVATransition] Mode=" << static_cast<int>(Mode) << "\n";
}

EPlayerMode EVATransitionController::GetMode() const
{
    return Mode;
}

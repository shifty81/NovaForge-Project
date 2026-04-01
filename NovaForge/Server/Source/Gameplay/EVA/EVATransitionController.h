#pragma once

#include "Gameplay/EVA/PlayerModeTypes.h"

class AirlockController;
class EVAMovementController;
class TetherController;
class SurvivalController;

class EVATransitionController
{
public:
    bool Initialize(AirlockController& InAirlock, EVAMovementController& InMovement, TetherController& InTether, SurvivalController& InSurvival);
    bool TryExitShip();
    bool TryReturnToShip();
    void Tick(float DeltaTime);

    EPlayerMode GetMode() const;

private:
    AirlockController* Airlock = nullptr;
    EVAMovementController* Movement = nullptr;
    TetherController* Tether = nullptr;
    SurvivalController* Survival = nullptr;
    EPlayerMode Mode = EPlayerMode::Interior;
};

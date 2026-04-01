#pragma once

#include "Gameplay/Environment/SurvivalTypes.h"

class TetherController;

class SurvivalController
{
public:
    bool Initialize();
    void SetInPressurizedInterior(bool bInterior);
    void Tick(float DeltaTime, const TetherController& Tether);

    const SurvivalState& GetState() const;

private:
    SurvivalState State;
};

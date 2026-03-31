#pragma once

#include "Gameplay/Airlock/AirlockTypes.h"

class AirlockController
{
public:
    bool Initialize();
    bool RequestCycleToExterior();
    bool RequestCycleToInterior();
    void Tick(float DeltaTime);

    bool CanExitToEVA() const;
    bool CanReturnToInterior() const;
    const AirlockRuntimeState& GetState() const;

private:
    AirlockRuntimeState Runtime;
};

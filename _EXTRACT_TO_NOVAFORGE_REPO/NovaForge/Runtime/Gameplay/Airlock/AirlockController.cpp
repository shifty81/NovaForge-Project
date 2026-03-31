#include "Gameplay/Airlock/AirlockController.h"
#include <iostream>

bool AirlockController::Initialize()
{
    std::cout << "[Airlock] Initialize\n";
    Runtime.State = EAirlockRuntimeState::IdleInteriorReady;
    Runtime.bInnerDoorOpen = true;
    Runtime.bOuterDoorOpen = false;
    Runtime.bChamberPressurized = true;
    return true;
}

bool AirlockController::RequestCycleToExterior()
{
    if (Runtime.State != EAirlockRuntimeState::IdleInteriorReady)
    {
        return false;
    }

    Runtime.State = EAirlockRuntimeState::SealingInnerDoor;
    Runtime.bInnerDoorOpen = false;
    Runtime.CycleProgress = 0.0f;
    std::cout << "[Airlock] Cycle requested to exterior\n";
    return true;
}

bool AirlockController::RequestCycleToInterior()
{
    if (Runtime.State != EAirlockRuntimeState::ExteriorReady)
    {
        return false;
    }

    Runtime.bOuterDoorOpen = false;
    Runtime.State = EAirlockRuntimeState::Repressurizing;
    Runtime.CycleProgress = 0.0f;
    std::cout << "[Airlock] Cycle requested to interior\n";
    return true;
}

void AirlockController::Tick(float DeltaTime)
{
    Runtime.CycleProgress += DeltaTime;

    switch (Runtime.State)
    {
        case EAirlockRuntimeState::SealingInnerDoor:
            if (Runtime.CycleProgress >= 1.0f)
            {
                Runtime.State = EAirlockRuntimeState::Depressurizing;
                Runtime.CycleProgress = 0.0f;
            }
            break;
        case EAirlockRuntimeState::Depressurizing:
            if (Runtime.CycleProgress >= 1.0f)
            {
                Runtime.bChamberPressurized = false;
                Runtime.bOuterDoorOpen = true;
                Runtime.State = EAirlockRuntimeState::ExteriorReady;
                Runtime.CycleProgress = 0.0f;
            }
            break;
        case EAirlockRuntimeState::Repressurizing:
            if (Runtime.CycleProgress >= 1.0f)
            {
                Runtime.bChamberPressurized = true;
                Runtime.bInnerDoorOpen = true;
                Runtime.State = EAirlockRuntimeState::IdleCycleComplete;
                Runtime.CycleProgress = 0.0f;
            }
            break;
        case EAirlockRuntimeState::IdleCycleComplete:
            Runtime.State = EAirlockRuntimeState::IdleInteriorReady;
            break;
        default:
            break;
    }

    std::cout << "[Airlock] State=" << static_cast<int>(Runtime.State)
              << " Pressurized=" << (Runtime.bChamberPressurized ? "Yes" : "No")
              << " InnerOpen=" << (Runtime.bInnerDoorOpen ? "Yes" : "No")
              << " OuterOpen=" << (Runtime.bOuterDoorOpen ? "Yes" : "No")
              << "\n";
}

bool AirlockController::CanExitToEVA() const
{
    return Runtime.State == EAirlockRuntimeState::ExteriorReady && Runtime.bOuterDoorOpen;
}

bool AirlockController::CanReturnToInterior() const
{
    return Runtime.State == EAirlockRuntimeState::IdleInteriorReady && Runtime.bInnerDoorOpen && Runtime.bChamberPressurized;
}

const AirlockRuntimeState& AirlockController::GetState() const
{
    return Runtime;
}

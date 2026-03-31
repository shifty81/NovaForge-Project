#pragma once

enum class EAirlockRuntimeState
{
    IdleInteriorReady,
    SealingInnerDoor,
    Depressurizing,
    ExteriorReady,
    Repressurizing,
    IdleCycleComplete
};

struct AirlockRuntimeState
{
    EAirlockRuntimeState State = EAirlockRuntimeState::IdleInteriorReady;
    bool bInnerDoorOpen = false;
    bool bOuterDoorOpen = false;
    bool bChamberPressurized = true;
    float CycleProgress = 0.0f;
};

#pragma once

#include <string>
#include <vector>

struct RoomEnvironmentState
{
    std::string RoomId;
    bool bHasOxygen = true;
    bool bHasGravity = true;
    bool bPressurized = true;
};

struct DoorModuleState
{
    std::string ModuleId;
    bool bOpen = false;
    bool bLocked = false;
};

struct ContainerModuleState
{
    std::string ModuleId;
    int SlotCount = 16;
};

struct ReactorPanelState
{
    std::string ModuleId;
    bool bPowered = true;
    float HeatLevel = 0.15f;
};

enum class EAirlockState
{
    IdleClosed,
    CyclingToExterior,
    ExteriorReady,
    CyclingToInterior
};

struct AirlockState
{
    std::string ModuleId;
    EAirlockState State = EAirlockState::IdleClosed;
    bool bInnerDoorOpen = false;
    bool bOuterDoorOpen = false;
};

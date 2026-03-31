#pragma once

#include "Input/InputActions.h"
#include <string>
#include <vector>

struct InputBinding
{
    EInputAction Action = EInputAction::None;
    std::string Key;
};

struct InputFrameState
{
    bool MoveForward = false;
    bool MoveBackward = false;
    bool MoveLeft = false;
    bool MoveRight = false;
    bool ToggleInventory = false;
    bool ToggleCrafting = false;
    bool ToggleMissionLog = false;
    bool Interact = false;
    bool ToggleToolOverlay = false;
    float LookYawDelta = 0.0f;
    float LookPitchDelta = 0.0f;
};

struct InputConfig
{
    std::vector<InputBinding> Bindings;
};

#pragma once

#include "UI/RuntimeHUDState.h"
#include <string>

class RuntimeHUDController
{
public:
    bool Initialize();
    void ToggleInventory(RuntimeHUDState& State);
    void ToggleCrafting(RuntimeHUDState& State);
    void ToggleMissionLog(RuntimeHUDState& State);
    void PushMessage(RuntimeHUDState& State, const std::string& Message);
};

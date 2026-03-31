#pragma once

#include "UI/RuntimeUIState.h"

class RuntimeUIHooks
{
public:
    bool Initialize();
    void ToggleInventory(RuntimeUIState& State);
    void ToggleCrafting(RuntimeUIState& State);
    void ToggleMissionLog(RuntimeUIState& State);
};

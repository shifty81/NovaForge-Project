#include "UI/RuntimeUIHooks.h"
#include <iostream>

bool RuntimeUIHooks::Initialize()
{
    std::cout << "[RuntimeUIHooks] Initialize\n";
    return true;
}

void RuntimeUIHooks::ToggleInventory(RuntimeUIState& State)
{
    State.bInventoryOpen = !State.bInventoryOpen;
    std::cout << "[RuntimeUIHooks] Inventory=" << (State.bInventoryOpen ? "Open" : "Closed") << "\n";
}

void RuntimeUIHooks::ToggleCrafting(RuntimeUIState& State)
{
    State.bCraftingOpen = !State.bCraftingOpen;
    std::cout << "[RuntimeUIHooks] Crafting=" << (State.bCraftingOpen ? "Open" : "Closed") << "\n";
}

void RuntimeUIHooks::ToggleMissionLog(RuntimeUIState& State)
{
    State.bMissionLogOpen = !State.bMissionLogOpen;
    std::cout << "[RuntimeUIHooks] MissionLog=" << (State.bMissionLogOpen ? "Open" : "Closed") << "\n";
}

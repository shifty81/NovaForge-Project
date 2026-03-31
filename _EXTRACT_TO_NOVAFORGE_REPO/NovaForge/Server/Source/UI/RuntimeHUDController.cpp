#include "UI/RuntimeHUDController.h"
#include <iostream>

bool RuntimeHUDController::Initialize()
{
    std::cout << "[RuntimeHUD] Initialize\n";
    return true;
}

void RuntimeHUDController::ToggleInventory(RuntimeHUDState& State)
{
    State.bInventoryOpen = !State.bInventoryOpen;
    PushMessage(State, std::string("Inventory ") + (State.bInventoryOpen ? "Opened" : "Closed"));
}

void RuntimeHUDController::ToggleCrafting(RuntimeHUDState& State)
{
    State.bCraftingOpen = !State.bCraftingOpen;
    PushMessage(State, std::string("Crafting ") + (State.bCraftingOpen ? "Opened" : "Closed"));
}

void RuntimeHUDController::ToggleMissionLog(RuntimeHUDState& State)
{
    State.bMissionLogOpen = !State.bMissionLogOpen;
    PushMessage(State, std::string("Mission Log ") + (State.bMissionLogOpen ? "Opened" : "Closed"));
}

void RuntimeHUDController::PushMessage(RuntimeHUDState& State, const std::string& Message)
{
    State.Messages.push_back({Message});
}

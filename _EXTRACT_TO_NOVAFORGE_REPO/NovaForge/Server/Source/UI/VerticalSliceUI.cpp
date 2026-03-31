#include "UI/VerticalSliceUI.h"
#include <iostream>

bool VerticalSliceUI::Initialize()
{
    std::cout << "[VerticalSliceUI] Initialize\n";
    return true;
}

void VerticalSliceUI::Tick(float)
{
    std::cout << "[VerticalSliceUI] HUD=" << (State.bHUDVisible ? "On" : "Off")
              << " Inv=" << (State.bInventoryVisible ? "On" : "Off")
              << " Contracts=" << (State.bContractsVisible ? "On" : "Off")
              << " Station=" << (State.bStationServicesVisible ? "On" : "Off")
              << "\n";

    for (const auto& Message : State.Messages)
    {
        std::cout << "  UI> " << Message << "\n";
    }
    State.Messages.clear();
}

void VerticalSliceUI::Shutdown()
{
    std::cout << "[VerticalSliceUI] Shutdown\n";
}

void VerticalSliceUI::ToggleInventory() { State.bInventoryVisible = !State.bInventoryVisible; }
void VerticalSliceUI::ToggleContracts() { State.bContractsVisible = !State.bContractsVisible; }
void VerticalSliceUI::ToggleStationServices() { State.bStationServicesVisible = !State.bStationServicesVisible; }
void VerticalSliceUI::ShowHUDMessage(const std::string& Message) { State.Messages.push_back(Message); }
const UIScreenState& VerticalSliceUI::GetState() const { return State; }

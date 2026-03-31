#include "UI/RuntimeHUDRenderer.h"
#include <iostream>

void RuntimeHUDRenderer::Render(const RuntimeHUDState& State) const
{
    std::cout << "[HUD] Inventory=" << (State.bInventoryOpen ? "Open" : "Closed")
              << " Crafting=" << (State.bCraftingOpen ? "Open" : "Closed")
              << " Missions=" << (State.bMissionLogOpen ? "Open" : "Closed")
              << "\n";

    for (const auto& Msg : State.Messages)
    {
        std::cout << "  HUD> " << Msg.Text << "\n";
    }
}

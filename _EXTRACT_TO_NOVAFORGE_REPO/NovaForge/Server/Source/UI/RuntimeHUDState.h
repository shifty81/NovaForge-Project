#pragma once

#include <string>
#include <vector>

struct HUDMessage
{
    std::string Text;
};

struct RuntimeHUDState
{
    bool bInventoryOpen = false;
    bool bCraftingOpen = false;
    bool bMissionLogOpen = false;
    std::vector<HUDMessage> Messages;
};

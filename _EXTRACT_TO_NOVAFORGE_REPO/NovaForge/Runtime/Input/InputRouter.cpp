#include "Input/InputRouter.h"

bool InputRouter::Initialize(const InputConfig& InConfig)
{
    Config = InConfig;
    return true;
}

InputFrameState InputRouter::BuildFrameState(const std::vector<std::string>& PressedKeys, float LookYawDelta, float LookPitchDelta) const
{
    InputFrameState Frame;
    Frame.LookYawDelta = LookYawDelta;
    Frame.LookPitchDelta = LookPitchDelta;

    auto has_key = [&](const char* key)
    {
        for (const auto& k : PressedKeys)
        {
            if (k == key) return true;
        }
        return false;
    };

    Frame.MoveForward = has_key("W");
    Frame.MoveBackward = has_key("S");
    Frame.MoveLeft = has_key("A");
    Frame.MoveRight = has_key("D");
    Frame.ToggleInventory = has_key("Tab");
    Frame.ToggleCrafting = has_key("C");
    Frame.ToggleMissionLog = has_key("J");
    Frame.Interact = has_key("E");
    Frame.ToggleToolOverlay = has_key("F12");
    return Frame;
}

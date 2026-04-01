#include "Input/InputConfig.h"

InputConfig InputConfigFactory::CreateDefault()
{
    return {
        {
            {EInputAction::MoveForward, "W"},
            {EInputAction::MoveBackward, "S"},
            {EInputAction::MoveLeft, "A"},
            {EInputAction::MoveRight, "D"},
            {EInputAction::ToggleInventory, "Tab"},
            {EInputAction::ToggleCrafting, "C"},
            {EInputAction::ToggleMissionLog, "J"},
            {EInputAction::Interact, "E"},
            {EInputAction::ToggleToolOverlay, "F12"}
        }
    };
}

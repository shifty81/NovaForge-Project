#pragma once

#include "Input/InputTypes.h"
#include "UI/RuntimeHUDState.h"
#include <string>

class PlayerController;
class InteractionSystem;
class RuntimeHUDController;
class ToolingSubsystem;

class GameplaySessionController
{
public:
    bool Initialize(PlayerController& InPlayer, InteractionSystem& InInteraction, RuntimeHUDController& InHUD, ToolingSubsystem& InTooling);
    void ProcessFrame(const InputFrameState& Frame, RuntimeHUDState& HUDState);

private:
    PlayerController* Player = nullptr;
    InteractionSystem* Interaction = nullptr;
    RuntimeHUDController* HUD = nullptr;
    ToolingSubsystem* Tooling = nullptr;
};

#include "Gameplay/GameplaySessionController.h"
#include "Gameplay/InteractionSystem.h"
#include "Gameplay/PlayerController.h"
#include "Tooling/ToolingSubsystem.h"
#include "UI/RuntimeHUDController.h"

bool GameplaySessionController::Initialize(PlayerController& InPlayer, InteractionSystem& InInteraction, RuntimeHUDController& InHUD, ToolingSubsystem& InTooling)
{
    Player = &InPlayer;
    Interaction = &InInteraction;
    HUD = &InHUD;
    Tooling = &InTooling;
    return true;
}

void GameplaySessionController::ProcessFrame(const InputFrameState& Frame, RuntimeHUDState& HUDState)
{
    if (Frame.ToggleInventory) HUD->ToggleInventory(HUDState);
    if (Frame.ToggleCrafting) HUD->ToggleCrafting(HUDState);
    if (Frame.ToggleMissionLog) HUD->ToggleMissionLog(HUDState);
    if (Frame.ToggleToolOverlay) Tooling->ToggleOverlay();

    float Forward = 0.0f;
    float Right = 0.0f;
    if (Frame.MoveForward) Forward += 1.0f;
    if (Frame.MoveBackward) Forward -= 1.0f;
    if (Frame.MoveRight) Right += 1.0f;
    if (Frame.MoveLeft) Right -= 1.0f;

    if (Forward != 0.0f || Right != 0.0f)
    {
        Player->Move(Forward, Right, 0.0f);
    }

    if (Frame.LookYawDelta != 0.0f || Frame.LookPitchDelta != 0.0f)
    {
        Player->Look(Frame.LookYawDelta, Frame.LookPitchDelta);
    }

    if (Frame.Interact)
    {
        Interaction->TryInteract("focused_target", "interior_reactor_panel");
        HUD->PushMessage(HUDState, "Interact pressed");
    }
}

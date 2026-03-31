#include "Gameplay/Characters/Core/CharacterStateAuthority.h"
#include "Gameplay/Characters/Core/CharacterTransitionRules.h"
#include "Gameplay/Characters/Animation/AnimationLayerSystem.h"
#include "Gameplay/Characters/IK/IKSystem.h"
#include "Gameplay/Characters/FPS/FPSPresentationSystem.h"
#include "Gameplay/Characters/Editor/CharacterEditorSystem.h"
#include "Gameplay/Characters/Tools/ToolInteractionShell.h"
#include "Gameplay/Characters/CharacterTypes.h"
#include <iostream>

int main()
{
    CharacterStateAuthority Authority;
    AnimationLayerSystem AnimationLayers;
    IKSystem IK;
    FPSPresentationSystem FPS;
    CharacterEditorSystem Editor;
    MiningToolContract MiningTool;

    Authority.Initialize();
    AnimationLayers.Initialize();
    IK.Initialize();
    FPS.Initialize();
    Editor.Initialize();

    AuthoritativeCharacterState Player;
    Player.CharacterId = "player_character_001";
    Player.DisplayName = "Player Character";
    Player.MovementMode = ECharacterMovementMode::FPS;
    Player.Equipment.EquippedItemIds = {"helmet_basic", "mining_tool_basic"};

    Authority.RegisterCharacter(Player);
    AnimationLayers.RegisterCharacter(Player.CharacterId);
    IK.RegisterCharacter(Player.CharacterId);
    FPS.RegisterCharacter(Player.CharacterId);

    MovementIntent Intent;
    Intent.Forward = 1.0f;
    Intent.LookYaw = 8.0f;
    Intent.LookPitch = -12.0f;
    Intent.bInteract = true;

    Authority.ApplyIntent(Player.CharacterId, Intent);
    Authority.Tick(1.0f / 60.0f);

    const auto* State = Authority.Find(Player.CharacterId);
    if (State)
    {
        AnimationLayers.EvaluateFromCharacterState(*State);
        IK.EvaluateFromCharacterState(*State);
        FPS.EvaluateFromCharacterState(*State);
    }

    CharacterTransitionContext Ctx;
    Ctx.bInAirlockExitZone = true;
    if (CharacterTransitionRules::CanTransition(ECharacterMovementMode::FPS, ECharacterMovementMode::EVA, Ctx))
    {
        auto* Mutable = Authority.FindMutable(Player.CharacterId);
        if (Mutable)
        {
            Mutable->MovementMode = ECharacterMovementMode::EVA;
        }
    }

    MiningTool.OnEquip(Player.CharacterId);
    MiningTool.OnStartUse(Player.CharacterId);
    MiningTool.OnTickUse(Player.CharacterId, 1.0f / 60.0f);
    MiningTool.OnStopUse(Player.CharacterId);

    Editor.OpenEditor(Player.CharacterId);
    Editor.SetHelmet("helmet_basic");
    Editor.SetBackpack("backpack_mk1");
    Editor.SetPrimaryColor("industrial_gray");
    Editor.SetAccentColor("signal_orange");

    AnimationLayers.Tick(1.0f / 60.0f);
    IK.Tick(1.0f / 60.0f);
    FPS.Tick(1.0f / 60.0f);

    std::cout << "[CharacterPhase2Bootstrap] Complete\n";
    return 0;
}

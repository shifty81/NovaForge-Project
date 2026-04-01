#include "Characters/CharacterControllerShell.h"
#include "Characters/CharacterSystem.h"
#include "Characters/Equipment/EquipmentSystem.h"
#include "Characters/Animation/AnimationController.h"
#include "Characters/Mech/MechPossessionSystem.h"
#include <iostream>

bool CharacterControllerShell::Initialize(CharacterSystem& InCharacters,
                                          EquipmentSystem& InEquipment,
                                          AnimationController& InAnimation,
                                          MechPossessionSystem& InMech)
{
    Characters = &InCharacters;
    Equipment = &InEquipment;
    Animation = &InAnimation;
    Mech = &InMech;
    return true;
}

void CharacterControllerShell::BootstrapStarterCharacter()
{
    CharacterRigState Rig;
    Rig.CharacterId = "player_character_001";
    Rig.DisplayName = "Player Character";
    Rig.MovementMode = ECharacterMovementMode::FPS;
    Rig.Parts = {
        {"head_base", ECharacterBodyPartType::Head, "mesh_head_lowpoly", "mat_suit_base", true},
        {"torso_base", ECharacterBodyPartType::Torso, "mesh_torso_lowpoly", "mat_suit_base", true},
        {"arm_upper_l", ECharacterBodyPartType::UpperArmLeft, "mesh_arm_upper_l", "mat_suit_base", true},
        {"arm_upper_r", ECharacterBodyPartType::UpperArmRight, "mesh_arm_upper_r", "mat_suit_base", true},
        {"leg_upper_l", ECharacterBodyPartType::UpperLegLeft, "mesh_leg_upper_l", "mat_suit_base", true},
        {"leg_upper_r", ECharacterBodyPartType::UpperLegRight, "mesh_leg_upper_r", "mat_suit_base", true},
        {"backpack_base", ECharacterBodyPartType::Backpack, "mesh_backpack_lowpoly", "mat_suit_base", true}
    };
    Rig.Sockets = {
        {"socket_hand_r", "hand_r", "tool"},
        {"socket_backpack", "spine_back", "backpack"},
        {"socket_helmet", "head", "helmet"}
    };

    Characters->RegisterCharacter(Rig);
    Animation->RegisterCharacter(Rig.CharacterId);
    Mech->RegisterCharacter(Rig.CharacterId);

    Equipment->RegisterDefinition({"mining_tool_basic", "tool", "mesh_tool_miner", "socket_hand_r", "tool_tier1"});
    Equipment->RegisterDefinition({"helmet_basic", "helmet", "mesh_helmet_basic", "socket_helmet", "helmet_tier1"});
    Equipment->EquipItem(Rig.CharacterId, {"mining_tool_basic", "socket_hand_r", true});
    Equipment->EquipItem(Rig.CharacterId, {"helmet_basic", "socket_helmet", true});

    std::cout << "[CharacterControllerShell] Starter character bootstrapped\n";
}

void CharacterControllerShell::SetMode(const std::string& CharacterId, ECharacterMovementMode Mode)
{
    Characters->SetMovementMode(CharacterId, Mode);
    Animation->UpdateMovementState(CharacterId, Mode, 25.0f, Mode == ECharacterMovementMode::EVA);
}

void CharacterControllerShell::Tick(float DeltaTime)
{
    Characters->Tick(DeltaTime);
    Equipment->Tick(DeltaTime);
    Animation->Tick(DeltaTime);
    Mech->Tick(DeltaTime);
}

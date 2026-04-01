#include "Gameplay/Characters/CharacterSystem.h"
#include "Gameplay/Characters/CharacterControllerShell.h"
#include "Gameplay/Characters/Equipment/EquipmentSystem.h"
#include "Gameplay/Characters/Animation/AnimationController.h"
#include "Gameplay/Characters/Mech/MechPossessionSystem.h"
#include <iostream>

int main()
{
    CharacterSystem Characters;
    EquipmentSystem Equipment;
    AnimationController Animation;
    MechPossessionSystem Mech;
    CharacterControllerShell Shell;

    Characters.Initialize();
    Equipment.Initialize();
    Animation.Initialize();
    Mech.Initialize();
    Shell.Initialize(Characters, Equipment, Animation, Mech);

    Shell.BootstrapStarterCharacter();
    Shell.SetMode("player_character_001", ECharacterMovementMode::FPS);
    Shell.SetMode("player_character_001", ECharacterMovementMode::EVA);
    Mech.EnterMech("player_character_001", "mech_utility_001");
    Shell.SetMode("player_character_001", ECharacterMovementMode::Mech);
    Shell.Tick(1.0f / 60.0f);

    std::cout << "[CharacterBootstrap] Complete\n";
    return 0;
}

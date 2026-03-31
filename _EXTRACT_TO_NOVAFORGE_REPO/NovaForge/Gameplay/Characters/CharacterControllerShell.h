#pragma once

#include "Characters/CharacterTypes.h"

class CharacterSystem;
class EquipmentSystem;
class AnimationController;
class MechPossessionSystem;

class CharacterControllerShell
{
public:
    bool Initialize(CharacterSystem& InCharacters,
                    EquipmentSystem& InEquipment,
                    AnimationController& InAnimation,
                    MechPossessionSystem& InMech);

    void BootstrapStarterCharacter();
    void SetMode(const std::string& CharacterId, ECharacterMovementMode Mode);
    void Tick(float DeltaTime);

private:
    CharacterSystem* Characters = nullptr;
    EquipmentSystem* Equipment = nullptr;
    AnimationController* Animation = nullptr;
    MechPossessionSystem* Mech = nullptr;
};

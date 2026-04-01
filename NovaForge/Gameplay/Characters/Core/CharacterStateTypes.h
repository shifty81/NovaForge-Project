#pragma once

#include "Characters/CharacterTypes.h"
#include <string>
#include <vector>

struct MovementIntent
{
    float Forward = 0.0f;
    float Right = 0.0f;
    float Up = 0.0f;
    float LookYaw = 0.0f;
    float LookPitch = 0.0f;
    bool bInteract = false;
    bool bBoost = false;
};

struct CharacterEquipmentState
{
    std::vector<std::string> EquippedItemIds;
};

struct CharacterSuitState
{
    float Oxygen = 100.0f;
    float Power = 100.0f;
    bool bPressurized = true;
};

struct CharacterDamageState
{
    float Health = 100.0f;
    bool bLeftArmDamaged = false;
    bool bRightArmDamaged = false;
    bool bLeftLegDamaged = false;
    bool bRightLegDamaged = false;
};

struct AuthoritativeCharacterState
{
    std::string CharacterId;
    std::string DisplayName;
    ECharacterMovementMode MovementMode = ECharacterMovementMode::FPS;
    CharacterEquipmentState Equipment;
    CharacterSuitState Suit;
    CharacterDamageState Damage;
    MovementIntent Intent;
    float PositionX = 0.0f;
    float PositionY = 0.0f;
    float PositionZ = 0.0f;
    float VelocityX = 0.0f;
    float VelocityY = 0.0f;
    float VelocityZ = 0.0f;
};

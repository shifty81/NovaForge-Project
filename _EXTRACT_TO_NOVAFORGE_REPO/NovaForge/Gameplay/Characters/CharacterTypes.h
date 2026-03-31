#pragma once

#include <string>
#include <vector>

enum class ECharacterBodyPartType
{
    Head,
    Torso,
    UpperArmLeft,
    UpperArmRight,
    LowerArmLeft,
    LowerArmRight,
    HandLeft,
    HandRight,
    UpperLegLeft,
    UpperLegRight,
    LowerLegLeft,
    LowerLegRight,
    FootLeft,
    FootRight,
    Backpack
};

enum class ECharacterMovementMode
{
    FPS,
    EVA,
    Mech
};

struct CharacterBodyPart
{
    std::string PartId;
    ECharacterBodyPartType Type = ECharacterBodyPartType::Torso;
    std::string MeshId;
    std::string MaterialId;
    bool bVisible = true;
};

struct CharacterSocket
{
    std::string SocketId;
    std::string BoneName;
    std::string AllowedCategory;
};

struct CharacterRigState
{
    std::string CharacterId;
    std::string DisplayName;
    ECharacterMovementMode MovementMode = ECharacterMovementMode::FPS;
    std::vector<CharacterBodyPart> Parts;
    std::vector<CharacterSocket> Sockets;
};

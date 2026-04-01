#pragma once

#include <string>

enum class EIKTargetType
{
    HandRight,
    HandLeft,
    FootRight,
    FootLeft,
    LookTarget
};

struct IKTarget
{
    std::string CharacterId;
    EIKTargetType Type = EIKTargetType::HandRight;
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
    bool bActive = false;
};

#pragma once

#include <string>
#include <vector>

struct Vector3
{
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
};

struct TransformComponent
{
    Vector3 Position;
    float Pitch = 0.0f;
    float Yaw = 0.0f;
    float Roll = 0.0f;
};

struct PlayerComponent
{
    std::string PlayerId;
    std::string DisplayName;
    bool bIsLocalPlayer = true;
};

struct InventorySlot
{
    std::string ItemId;
    int Count = 0;
};

struct MissionProgress
{
    std::string MissionId;
    bool bAccepted = false;
    bool bCompleted = false;
    int ObjectiveCount = 0;
};

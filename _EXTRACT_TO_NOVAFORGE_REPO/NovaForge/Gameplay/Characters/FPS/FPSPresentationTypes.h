#pragma once

#include <string>

struct FPSPresentationState
{
    std::string CharacterId;
    bool bFullBodyAwarenessEnabled = true;
    bool bSeparateHandRigEnabled = true;
    bool bShowFeetWhenLookingDown = true;
    float CameraOffsetX = 0.0f;
    float CameraOffsetY = 0.0f;
    float CameraOffsetZ = 168.0f;
    float HandIdleSwayWeight = 0.35f;
};

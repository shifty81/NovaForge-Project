#pragma once

struct EVAState
{
    bool bActive = false;
    bool bDampeningEnabled = true;
    float ThrustForward = 0.0f;
    float ThrustRight = 0.0f;
    float ThrustUp = 0.0f;
    float VelocityX = 0.0f;
    float VelocityY = 0.0f;
    float VelocityZ = 0.0f;
};

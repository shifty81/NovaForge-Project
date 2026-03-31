#pragma once

struct TetherState
{
    bool bActive = false;
    float CurrentLength = 0.0f;
    float MaxLength = 50.0f;
    bool bSupplyingOxygen = false;
    bool bSupplyingPower = false;
};

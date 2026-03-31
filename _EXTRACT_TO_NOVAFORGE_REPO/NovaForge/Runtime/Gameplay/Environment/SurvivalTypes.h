#pragma once

struct SurvivalState
{
    float SuitOxygen = 100.0f;
    float SuitPower = 100.0f;
    bool bReceivingTetherOxygen = false;
    bool bReceivingTetherPower = false;
    bool bInPressurizedInterior = true;
};

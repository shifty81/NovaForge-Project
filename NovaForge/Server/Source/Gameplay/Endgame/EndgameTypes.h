#pragma once

#include <string>

enum class EGateState
{
    Dormant,
    Charging,
    Ready,
    JumpTriggered,
    Completed
};

struct EndgameGateState
{
    std::string GateId;
    EGateState State = EGateState::Dormant;
    bool bTitanRequired = true;
    bool bJumpSuccessful = false;
};

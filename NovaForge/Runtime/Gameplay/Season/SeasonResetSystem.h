#pragma once

#include "Gameplay/Season/SeasonTypes.h"
#include <string>

class EndgameGateSystem;

class SeasonResetSystem
{
public:
    bool Initialize(EndgameGateSystem& InGateSystem);

    void ApplyPolicy(const SeasonPolicy& Policy);
    void SetDaysRemaining(int Days);
    void AddCarryoverReward(const CarryoverReward& Reward);
    void TriggerSeasonEnding();
    bool TryFinalizeReset(const std::string& GateId);

    const SeasonState& GetState() const;
    void Tick(float DeltaTime);

private:
    EndgameGateSystem* GateSystem = nullptr;
    SeasonState State;
};

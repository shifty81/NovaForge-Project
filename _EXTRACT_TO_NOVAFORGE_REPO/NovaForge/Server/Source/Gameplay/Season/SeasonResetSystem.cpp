#include "Gameplay/Season/SeasonResetSystem.h"
#include "Gameplay/Endgame/EndgameGateSystem.h"
#include <iostream>

bool SeasonResetSystem::Initialize(EndgameGateSystem& InGateSystem)
{
    GateSystem = &InGateSystem;
    std::cout << "[Season] Initialize\n";
    return true;
}

void SeasonResetSystem::SetDaysRemaining(int Days)
{
    State.DaysRemaining = Days;
}

void SeasonResetSystem::AddCarryoverReward(const CarryoverReward& Reward)
{
    State.Rewards.push_back(Reward);
    std::cout << "[Season] Added carryover reward " << Reward.RewardId << "\n";
}

void SeasonResetSystem::TriggerSeasonEnding()
{
    State.State = ESeasonState::Ending;
    std::cout << "[Season] Season ending triggered\n";
}

bool SeasonResetSystem::TryFinalizeReset(const std::string& GateId)
{
    if (!GateSystem)
    {
        return false;
    }

    const auto* Gate = GateSystem->FindGate(GateId);
    if (!Gate || !Gate->bJumpSuccessful)
    {
        std::cout << "[Season] Reset denied, jump not successful\n";
        return false;
    }

    State.State = ESeasonState::ResetPending;
    State.SeasonNumber += 1;
    State.DaysRemaining = 90;
    State.State = ESeasonState::ResetComplete;

    std::cout << "[Season] Reset complete. New season=" << State.SeasonNumber << "\n";
    return true;
}

const SeasonState& SeasonResetSystem::GetState() const
{
    return State;
}

void SeasonResetSystem::Tick(float)
{
    std::cout << "[Season] Season=" << State.SeasonNumber
              << " DaysRemaining=" << State.DaysRemaining
              << " Rewards=" << State.Rewards.size() << "\n";
}

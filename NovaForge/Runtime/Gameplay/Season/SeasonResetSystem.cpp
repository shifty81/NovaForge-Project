#include "Gameplay/Season/SeasonResetSystem.h"
#include "Gameplay/Endgame/EndgameGateSystem.h"
#include <iostream>

bool SeasonResetSystem::Initialize(EndgameGateSystem& InGateSystem)
{
    GateSystem = &InGateSystem;
    State.Policy = SeasonPolicy{};
    State.DaysRemaining = State.Policy.LengthDays;
    std::cout << "[Season] Initialize LengthDays=" << State.Policy.LengthDays
              << " ServerAuthority=" << (State.Policy.bServerAuthoritative ? "true" : "false")
              << "\n";
    return true;
}

void SeasonResetSystem::ApplyPolicy(const SeasonPolicy& Policy)
{
    State.Policy = Policy;
    if (State.DaysRemaining <= 0 || State.DaysRemaining > Policy.LengthDays)
    {
        State.DaysRemaining = Policy.LengthDays;
    }

    std::cout << "[Season] Policy applied LengthDays=" << State.Policy.LengthDays
              << " WarningWindowDays=" << State.Policy.WarningWindowDays
              << " ServerAuthority=" << (State.Policy.bServerAuthoritative ? "true" : "false")
              << "\n";
}

void SeasonResetSystem::SetDaysRemaining(int Days)
{
    State.DaysRemaining = Days;
}

void SeasonResetSystem::AddCarryoverReward(const CarryoverReward& Reward)
{
    if (!State.Policy.bCarryoverRewardsEnabled)
    {
        return;
    }

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
    State.DaysRemaining = State.Policy.LengthDays;
    State.State = ESeasonState::ResetComplete;

    std::cout << "[Season] Reset complete. New season=" << State.SeasonNumber
              << " LengthDays=" << State.Policy.LengthDays << "\n";
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
              << " LengthDays=" << State.Policy.LengthDays
              << " Rewards=" << State.Rewards.size()
              << "\n";

    if (State.Policy.bEnabled && State.DaysRemaining <= State.Policy.WarningWindowDays)
    {
        std::cout << "[Season] Warning window active\n";
    }
}

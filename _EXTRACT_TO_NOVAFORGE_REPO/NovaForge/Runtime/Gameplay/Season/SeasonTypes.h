#pragma once

#include <string>
#include <vector>

enum class ESeasonState
{
    Active,
    Ending,
    ResetPending,
    ResetComplete
};

struct CarryoverReward
{
    std::string RewardId;
    std::string Category;
    int Value = 0;
};

struct SeasonPolicy
{
    bool bEnabled = true;
    bool bServerAuthoritative = true;
    int LengthDays = 180;
    int WarningWindowDays = 14;
    bool bCarryoverRewardsEnabled = true;
};

struct SeasonState
{
    int SeasonNumber = 1;
    ESeasonState State = ESeasonState::Active;
    int DaysRemaining = 180;
    std::vector<CarryoverReward> Rewards;
    SeasonPolicy Policy;
};

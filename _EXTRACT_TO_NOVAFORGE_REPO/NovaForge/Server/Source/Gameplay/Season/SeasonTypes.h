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

struct SeasonState
{
    int SeasonNumber = 1;
    ESeasonState State = ESeasonState::Active;
    int DaysRemaining = 90;
    std::vector<CarryoverReward> Rewards;
};

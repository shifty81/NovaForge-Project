#pragma once

#include <string>
#include <vector>

struct FactionStanding
{
    std::string FactionId;
    int Reputation = 0;
    int Influence = 0;
};

struct FactionState
{
    std::vector<FactionStanding> Standings;
};

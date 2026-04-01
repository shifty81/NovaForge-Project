#pragma once

struct ClientSeasonConfig
{
    bool bShowSeasonUI = true;
    bool bShowSeasonWarnings = true;
    int PreferredWarningDays = 14;
};

struct ServerSeasonConfig
{
    bool bSeasonsEnabled = true;
    bool bServerAuthoritativeSeasons = true;
    int SeasonLengthDays = 180;
    int SeasonEndWarningWindowDays = 14;
    bool bCarryoverRewardsEnabled = true;
};

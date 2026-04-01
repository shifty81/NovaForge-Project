#pragma once

#include <string>
#include <vector>

struct TradeRoute
{
    std::string RouteId;
    std::string SourceFactionId;
    std::string DestinationFactionId;
    std::string ItemId;
    int Volume = 0;
};

struct TradeState
{
    std::vector<TradeRoute> Routes;
};

#pragma once

#include <string>
#include <vector>

struct MarketItemValue
{
    std::string ItemId;
    int BaseValue = 0;
    float DemandModifier = 1.0f;
    float SupplyModifier = 1.0f;
};

struct EconomyState
{
    std::vector<MarketItemValue> MarketValues;
};

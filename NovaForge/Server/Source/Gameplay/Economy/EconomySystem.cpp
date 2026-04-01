#include "Gameplay/Economy/EconomySystem.h"
#include <iostream>

bool EconomySystem::Initialize()
{
    std::cout << "[Economy] Initialize\n";
    return true;
}

void EconomySystem::RegisterValue(const MarketItemValue& Value)
{
    State.MarketValues.push_back(Value);
    std::cout << "[Economy] Registered value for " << Value.ItemId << "\n";
}

int EconomySystem::GetCurrentItemValue(const std::string& ItemId) const
{
    for (const auto& Entry : State.MarketValues)
    {
        if (Entry.ItemId == ItemId)
        {
            const float FinalValue = static_cast<float>(Entry.BaseValue) * Entry.DemandModifier * Entry.SupplyModifier;
            return static_cast<int>(FinalValue);
        }
    }

    return 0;
}

void EconomySystem::Tick(float)
{
    std::cout << "[Economy] Entries=" << State.MarketValues.size() << "\n";
}

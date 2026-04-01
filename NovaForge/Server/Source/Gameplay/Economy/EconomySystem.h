#pragma once

#include "Gameplay/Economy/EconomyTypes.h"
#include <string>

class EconomySystem
{
public:
    bool Initialize();
    void RegisterValue(const MarketItemValue& Value);
    int GetCurrentItemValue(const std::string& ItemId) const;
    void Tick(float DeltaTime);

private:
    EconomyState State;
};

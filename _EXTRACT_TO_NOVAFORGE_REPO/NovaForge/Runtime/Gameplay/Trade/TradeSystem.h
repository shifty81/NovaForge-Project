#pragma once

#include "Gameplay/Trade/TradeTypes.h"
#include <string>

class TradeSystem
{
public:
    bool Initialize();
    void RegisterRoute(const TradeRoute& Route);
    const TradeRoute* FindRoute(const std::string& RouteId) const;
    void Tick(float DeltaTime);

private:
    TradeState State;
};

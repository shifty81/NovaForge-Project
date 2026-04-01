#include "Gameplay/Trade/TradeSystem.h"
#include <iostream>

bool TradeSystem::Initialize()
{
    std::cout << "[Trade] Initialize\n";
    return true;
}

void TradeSystem::RegisterRoute(const TradeRoute& Route)
{
    State.Routes.push_back(Route);
    std::cout << "[Trade] Registered route " << Route.RouteId << "\n";
}

const TradeRoute* TradeSystem::FindRoute(const std::string& RouteId) const
{
    for (const auto& Route : State.Routes)
    {
        if (Route.RouteId == RouteId)
        {
            return &Route;
        }
    }
    return nullptr;
}

void TradeSystem::Tick(float)
{
    std::cout << "[Trade] Routes=" << State.Routes.size() << "\n";
}

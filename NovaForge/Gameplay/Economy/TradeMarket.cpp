// TradeMarket.cpp
#include "TradeMarket.h"

namespace NovaForge::Gameplay::Economy
{

void TradeMarket::initialise() {}
void TradeMarket::shutdown()   {}

MarketSnapshot TradeMarket::queryStation(uint64_t stationId) const
{
    MarketSnapshot snap;
    snap.stationId = stationId;
    return snap;
}

bool  TradeMarket::placeOrder(const TradeOrder&)           { return true;  }
bool  TradeMarket::cancelOrder(uint64_t)                   { return true;  }
float TradeMarket::getBestPrice(const std::string&, bool)  const { return 0.0f; }

} // namespace NovaForge::Gameplay::Economy

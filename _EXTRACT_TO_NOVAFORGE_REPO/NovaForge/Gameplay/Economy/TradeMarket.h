// TradeMarket.h
// NovaForge trade market — price simulation, order book stubs, and resource listings.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Economy
{

struct TradeOrder
{
    uint64_t    orderId    = 0;
    uint64_t    stationId  = 0;
    std::string resourceId;
    float       price      = 0.0f;
    uint32_t    quantity   = 0;
    bool        isBuyOrder = false;
};

struct MarketSnapshot
{
    uint64_t              stationId = 0;
    std::vector<TradeOrder> orders;
};

class TradeMarket
{
public:
    TradeMarket()  = default;
    ~TradeMarket() = default;

    void initialise();
    void shutdown();

    MarketSnapshot queryStation(uint64_t stationId) const;
    bool placeOrder(const TradeOrder& order);
    bool cancelOrder(uint64_t orderId);
    float getBestPrice(const std::string& resourceId, bool buying) const;
};

} // namespace NovaForge::Gameplay::Economy

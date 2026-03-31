// MarketContractBridge.cpp
// NovaForge Integration — market/contract bridge.

#include "Integration/MarketContractBridge.h"

#include <algorithm>

namespace novaforge::integration {

bool MarketContractBridge::Initialize()
{
    m_activatedCount = 0;
    return true;
}

void MarketContractBridge::Shutdown()
{
    m_prices.clear();
    m_templates.clear();
    m_fleetNeeds.clear();
}

void MarketContractBridge::IngestPricePoint(const MarketPricePoint& point)
{
    for (auto& p : m_prices)
    {
        if (p.resourceId == point.resourceId &&
            p.stationId  == point.stationId)
        { p = point; return; }
    }
    m_prices.push_back(point);
}

void MarketContractBridge::ClearMarketData()
{
    m_prices.clear();
}

bool MarketContractBridge::HasPriceData(const std::string& resourceId) const
{
    return GetPricePoint(resourceId).has_value();
}

std::optional<MarketPricePoint> MarketContractBridge::GetPricePoint(
    const std::string& resourceId, const std::string& stationId) const
{
    for (const auto& p : m_prices)
    {
        if (p.resourceId == resourceId)
        {
            if (stationId.empty() || p.stationId == stationId) return p;
        }
    }
    return std::nullopt;
}

void MarketContractBridge::RegisterTemplate(
    const MarketContractTemplate& tmpl)
{
    for (auto& t : m_templates)
        if (t.templateId == tmpl.templateId) { t = tmpl; return; }
    m_templates.push_back(tmpl);
}

bool MarketContractBridge::HasTemplate(const std::string& templateId) const
{
    for (const auto& t : m_templates)
        if (t.templateId == templateId) return true;
    return false;
}

std::vector<ContractOpportunity>
MarketContractBridge::AnalyseOpportunities() const
{
    std::vector<ContractOpportunity> result;
    for (const auto& tmpl : m_templates)
    {
        auto price = GetPricePoint(tmpl.resourceId, tmpl.destStationId);
        if (!price) continue;

        float score = ScoreOpportunity(*price);
        if (score <= 0.f) continue;

        ContractOpportunity opp;
        opp.templateId       = tmpl.templateId;
        opp.resourceId       = tmpl.resourceId;
        opp.opportunityScore = score;
        opp.estimatedReward  = tmpl.baseReward * tmpl.urgencyMultiplier * (1.f + score);
        opp.reason           = "Market demand for " + tmpl.resourceId
                               + " at " + tmpl.destStationId
                               + " (score=" + std::to_string(score) + ")";
        result.push_back(opp);
    }

    // Also check fleet needs.
    for (const auto& need : m_fleetNeeds)
    {
        bool alreadyInList = false;
        for (const auto& o : result)
            if (o.resourceId == need) { alreadyInList = true; break; }
        if (alreadyInList) continue;

        ContractOpportunity opp;
        opp.resourceId       = need;
        opp.templateId       = "fleet_resupply_" + need;
        opp.opportunityScore = 0.8f;  // Fleet needs are high priority.
        opp.estimatedReward  = 1000.f;
        opp.reason           = "Fleet resupply need: " + need;
        result.push_back(opp);
    }

    // Sort by score descending.
    std::sort(result.begin(), result.end(),
              [](const ContractOpportunity& a, const ContractOpportunity& b)
              { return a.opportunityScore > b.opportunityScore; });

    return result;
}

std::vector<ContractOpportunity>
MarketContractBridge::TopOpportunities(size_t n) const
{
    auto all = AnalyseOpportunities();
    if (all.size() > n) all.resize(n);
    return all;
}

float MarketContractBridge::ComputeRewardMultiplier(
    const std::string& resourceId,
    const std::string& /*contractType*/) const
{
    auto price = GetPricePoint(resourceId);
    if (!price) return 1.0f;

    // High demand → high multiplier.
    if (price->demand > 0.f && price->supply > 0.f)
    {
        float ratio = price->demand / price->supply;
        if (ratio > 2.0f) return 2.0f;
        if (ratio > 1.0f) return ratio;
    }
    if (price->isHigh) return 1.5f;
    if (price->isLow)  return 0.8f;
    return 1.0f;
}

std::vector<std::string>
MarketContractBridge::ActivateTopContracts(size_t maxActive)
{
    auto opps = TopOpportunities(maxActive);
    std::vector<std::string> activated;
    for (const auto& opp : opps)
    {
        for (auto& t : m_templates)
        {
            if (t.templateId == opp.templateId)
            {
                t.isActive = true;
                activated.push_back(t.templateId);
                ++m_activatedCount;
                if (m_opportunityCb) m_opportunityCb(opp);
                break;
            }
        }
    }
    return activated;
}

void MarketContractBridge::Tick(float /*deltaSeconds*/)
{
    // Simulate minor price drift each tick (placeholder).
    // In a full system, price updates would come from TradeMarket.
}

std::vector<std::string>
MarketContractBridge::GetFleetResupplyNeeds() const
{
    return m_fleetNeeds;
}

float MarketContractBridge::ScoreOpportunity(
    const MarketPricePoint& price) const
{
    float score = 0.f;
    if (price.demand > price.supply)
        score += 0.5f * (price.demand - price.supply) / (price.demand + 1.f);
    if (price.isHigh) score += 0.3f;
    if (price.currentPrice > price.basePrice * 1.2f) score += 0.2f;
    if (score > 1.f) score = 1.f;
    return score;
}

} // namespace novaforge::integration

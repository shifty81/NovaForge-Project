// MarketContractBridge.h
// NovaForge Integration — bridges trade/market data into contract generation:
// dynamic contract pricing, market-driven opportunity levels, supply/demand
// contract seeding.

#pragma once
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace novaforge::integration {

// ---------------------------------------------------------------------------
// Market price snapshot (lightweight, for contract generation)
// ---------------------------------------------------------------------------

struct MarketPricePoint
{
    std::string resourceId;
    float       currentPrice    = 0.f;
    float       basePrice       = 0.f;
    float       supply          = 0.f;  ///< units available
    float       demand          = 0.f;  ///< units wanted
    std::string stationId;
    bool        isLow           = false;  ///< price below base (shortage)
    bool        isHigh          = false;  ///< price above base (surplus)
};

// ---------------------------------------------------------------------------
// Market-driven contract template
// ---------------------------------------------------------------------------

struct MarketContractTemplate
{
    std::string   templateId;
    std::string   contractType;    ///< "delivery", "mining", "hauling", "arbitrage"
    std::string   resourceId;      ///< primary resource
    std::string   sourceStationId;
    std::string   destStationId;
    float         baseReward       = 0.f;
    float         urgencyMultiplier = 1.f;  ///< scales reward for high-demand
    uint32_t      requiredQuantity = 0;
    bool          isActive         = false;
};

// ---------------------------------------------------------------------------
// Contract generation opportunity
// ---------------------------------------------------------------------------

struct ContractOpportunity
{
    std::string             templateId;
    std::string             resourceId;
    float                   opportunityScore = 0.f;  ///< 0–1
    float                   estimatedReward  = 0.f;
    std::string             reason;    ///< "High demand for iron ore at Alpha Station"
};

// ---------------------------------------------------------------------------
// MarketContractBridge
// ---------------------------------------------------------------------------

class MarketContractBridge
{
public:
    bool Initialize();
    void Shutdown();

    // ---- market data ingestion ----------------------------------------
    void IngestPricePoint   (const MarketPricePoint& point);
    void ClearMarketData    ();
    bool HasPriceData       (const std::string& resourceId) const;
    std::optional<MarketPricePoint> GetPricePoint(
        const std::string& resourceId,
        const std::string& stationId = "") const;
    const std::vector<MarketPricePoint>& GetAllPricePoints() const
    { return m_prices; }

    // ---- template registration ----------------------------------------
    void RegisterTemplate  (const MarketContractTemplate& tmpl);
    bool HasTemplate       (const std::string& templateId) const;
    size_t TemplateCount() const { return m_templates.size(); }

    // ---- opportunity analysis -----------------------------------------
    std::vector<ContractOpportunity> AnalyseOpportunities() const;
    std::vector<ContractOpportunity> TopOpportunities(size_t n = 5) const;

    // ---- dynamic contract generation ----------------------------------
    /// Generate contract reward multiplier based on current market conditions.
    float ComputeRewardMultiplier(const std::string& resourceId,
                                    const std::string& contractType) const;

    /// Activate the highest-opportunity templates for contract generation.
    std::vector<std::string> ActivateTopContracts(size_t maxActive = 5);

    // ---- tick (update market data, refresh opportunities) --------------
    void Tick(float deltaSeconds);

    // ---- fleet-contract connection ------------------------------------
    /// Get resource IDs most needed for fleet resupply.
    std::vector<std::string> GetFleetResupplyNeeds() const;
    void SetFleetResourceNeeds(const std::vector<std::string>& resourceIds)
    { m_fleetNeeds = resourceIds; }

    // ---- callback -------------------------------------------------------
    using OpportunityCallback = std::function<void(const ContractOpportunity&)>;
    void SetOpportunityCallback(OpportunityCallback cb)
    { m_opportunityCb = std::move(cb); }

    size_t ContractsActivated() const { return m_activatedCount; }

private:
    std::vector<MarketPricePoint>       m_prices;
    std::vector<MarketContractTemplate> m_templates;
    std::vector<std::string>            m_fleetNeeds;
    OpportunityCallback                 m_opportunityCb;
    size_t                              m_activatedCount = 0;

    float ScoreOpportunity(const MarketPricePoint& price) const;
};

} // namespace novaforge::integration

// ExplorationSystem.cpp
#include "ExplorationSystem.h"

namespace NovaForge::Gameplay::Exploration
{

void ExplorationSystem::initialise() {}
void ExplorationSystem::shutdown()   {}

ScanResult ExplorationSystem::scan(uint64_t, ScanType, float strength)
{
    ScanResult r;
    r.signalFound    = strength > 20.0f;
    r.signalStrength = strength;
    r.scanAccuracy   = strength;
    r.siteType       = "anomaly";
    r.siteId         = "site-stub-001";
    return r;
}

bool ExplorationSystem::beginSurvey(uint64_t, const std::string& siteId)
{
    for (const auto& s : sites_)
        if (s.siteId == siteId) return false; // already exists
    sites_.push_back({siteId, "anomaly", false, 0.0f});
    return true;
}

bool ExplorationSystem::completeSurvey(uint64_t, const std::string& siteId)
{
    for (auto& s : sites_)
        if (s.siteId == siteId) { s.fullySurveyed = true; s.explorationXP = 500.0f; return true; }
    return false;
}

std::vector<SurveySite> ExplorationSystem::listDiscovered(uint64_t) const
{
    return sites_;
}

} // namespace NovaForge::Gameplay::Exploration

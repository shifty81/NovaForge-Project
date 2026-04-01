// ExplorationSystem.h
// NovaForge exploration — scanning, probe deployment, ruin discovery, and survey flow.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Exploration
{

enum class ScanType : uint8_t { PassiveSensor, ActivePing, ProbeNetwork, DeepScan };

struct ScanResult
{
    bool        signalFound    = false;
    float       signalStrength = 0.0f; ///< 0.0–100.0
    std::string siteType;              ///< "ruin", "cache", "anomaly", "wormhole"
    std::string siteId;
    float       scanAccuracy   = 0.0f; ///< 0.0–100.0
};

struct SurveySite
{
    std::string siteId;
    std::string siteType;
    bool        fullySurveyed = false;
    float       explorationXP = 0.0f;
};

class ExplorationSystem
{
public:
    ExplorationSystem()  = default;
    ~ExplorationSystem() = default;

    void initialise();
    void shutdown();

    ScanResult scan(uint64_t sectorId, ScanType type, float scannerStrength);
    bool beginSurvey(uint64_t playerId, const std::string& siteId);
    bool completeSurvey(uint64_t playerId, const std::string& siteId);
    std::vector<SurveySite> listDiscovered(uint64_t playerId) const;

private:
    std::vector<SurveySite> sites_;
};

} // namespace NovaForge::Gameplay::Exploration

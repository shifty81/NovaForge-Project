// MiningSystem.h
// NovaForge mining — resource scanning, extraction, and yield calculations.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Mining
{

struct ResourceNode
{
    uint64_t    nodeId       = 0;
    std::string resourceType;
    float       totalYield   = 0.0f;   ///< total units available
    float       remaining    = 0.0f;
    bool        isDepleted   = false;
};

struct MiningResult
{
    bool        success      = false;
    float       unitsExtracted = 0.0f;
    bool        nodeDepleted = false;
    std::string resourceType;
};

class MiningSystem
{
public:
    MiningSystem()  = default;
    ~MiningSystem() = default;

    void initialise();
    void shutdown();

    uint64_t spawnNode(const std::string& resourceType, float totalYield);
    MiningResult mine(uint64_t nodeId, float extractionRate);
    bool scanNode(uint64_t nodeId, ResourceNode& outInfo) const;
    void deplete(uint64_t nodeId);

private:
    std::vector<ResourceNode> nodes_;
    uint64_t nextNodeId_ = 1;
};

} // namespace NovaForge::Gameplay::Mining

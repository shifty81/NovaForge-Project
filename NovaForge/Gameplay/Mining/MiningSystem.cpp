// MiningSystem.cpp
#include "MiningSystem.h"
#include <algorithm>

namespace NovaForge::Gameplay::Mining
{

void MiningSystem::initialise() {}
void MiningSystem::shutdown()   {}

uint64_t MiningSystem::spawnNode(const std::string& resourceType, float totalYield)
{
    ResourceNode n;
    n.nodeId       = nextNodeId_++;
    n.resourceType = resourceType;
    n.totalYield   = totalYield;
    n.remaining    = totalYield;
    nodes_.push_back(n);
    return n.nodeId;
}

MiningResult MiningSystem::mine(uint64_t nodeId, float rate)
{
    MiningResult r;
    for (auto& n : nodes_)
    {
        if (n.nodeId != nodeId || n.isDepleted) continue;
        float extracted = std::min(rate, n.remaining);
        n.remaining -= extracted;
        if (n.remaining <= 0.0f) { n.remaining = 0.0f; n.isDepleted = true; }
        r.success         = true;
        r.unitsExtracted  = extracted;
        r.nodeDepleted    = n.isDepleted;
        r.resourceType    = n.resourceType;
        return r;
    }
    return r;
}

bool MiningSystem::scanNode(uint64_t nodeId, ResourceNode& out) const
{
    for (const auto& n : nodes_)
        if (n.nodeId == nodeId) { out = n; return true; }
    return false;
}

void MiningSystem::deplete(uint64_t nodeId)
{
    for (auto& n : nodes_)
        if (n.nodeId == nodeId) { n.remaining = 0.0f; n.isDepleted = true; }
}

} // namespace NovaForge::Gameplay::Mining

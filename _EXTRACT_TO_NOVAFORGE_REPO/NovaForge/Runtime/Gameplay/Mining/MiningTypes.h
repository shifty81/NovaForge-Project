#pragma once
#include <string>

enum class EMiningNodeType
{
    Asteroid,
    OreVein,
    SurfaceDeposit
};

struct MiningNode
{
    std::string Id;
    EMiningNodeType Type;
    float ResourceAmount = 100.0f;
    std::string ResourceType;
    bool bDepleted = false;
};
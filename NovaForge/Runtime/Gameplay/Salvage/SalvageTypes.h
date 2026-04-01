#pragma once
#include <string>

enum class ESalvageNodeType
{
    Panel,
    Component,
    WreckChunk
};

struct SalvageNode
{
    std::string Id;
    ESalvageNodeType Type;
    float Integrity = 100.0f;
    std::string RequiredTool;
    std::string LootTableId;
    bool bDepleted = false;
};
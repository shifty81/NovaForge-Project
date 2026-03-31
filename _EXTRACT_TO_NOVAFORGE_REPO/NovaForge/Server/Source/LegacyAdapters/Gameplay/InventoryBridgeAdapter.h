#pragma once

#include <string>
#include <vector>

struct LegacyInventoryEntry
{
    std::string ItemName;
    int Count = 0;
};

struct MRInventoryEntry
{
    std::string ItemId;
    int Count = 0;
};

class InventoryBridgeAdapter
{
public:
    static std::vector<MRInventoryEntry> Convert(const std::vector<LegacyInventoryEntry>& LegacyEntries);
};

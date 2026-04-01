#include "LegacyAdapters/Gameplay/InventoryBridgeAdapter.h"
#include "LegacyAdapters/Data/DataConversionUtils.h"

std::vector<MRInventoryEntry> InventoryBridgeAdapter::Convert(const std::vector<LegacyInventoryEntry>& LegacyEntries)
{
    std::vector<MRInventoryEntry> Out;
    Out.reserve(LegacyEntries.size());

    for (const auto& Entry : LegacyEntries)
    {
        Out.push_back({DataConversionUtils::MakeId(Entry.ItemName), Entry.Count});
    }

    return Out;
}

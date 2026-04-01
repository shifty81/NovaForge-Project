#include "LegacyAdapters/Gameplay/CraftingBridgeAdapter.h"

std::string CraftingBridgeAdapter::ConvertLegacyMachineName(const std::string& LegacyMachineName)
{
    if (LegacyMachineName == "assembler")
    {
        return "assembler_tier1";
    }

    if (LegacyMachineName == "industrial_assembler")
    {
        return "assembler_tier2";
    }

    return LegacyMachineName;
}

#include "LegacyAdapters/Data/NovaForgeIngestionManifest.h"

std::vector<IngestedRecordInfo> NovaForgeIngestionManifest::GetSeededRecords()
{
    return {
        {"item", "steel_plate", "NovaForgeSample"},
        {"item", "copper_wire", "NovaForgeSample"},
        {"item", "micro_core", "NovaForgeSample"},
        {"recipe", "craft_reactor_mk1", "NovaForgeSample"},
        {"mission", "mission_salvage_derelict_001", "NovaForgeSample"},
        {"faction", "frontier_union", "NovaForgeSample"},
        {"loot", "loot_derelict_engineering_room", "NovaForgeSample"}
    };
}

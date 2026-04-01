#pragma once

#include "LegacyAdapters/Data/LegacyDataModels.h"
#include "LegacyAdapters/Data/MasterRepoDataModels.h"

class FactionAdapter
{
public:
    static MRFactionDefinition Convert(const LegacyFactionRecord& Legacy);
};

#pragma once

#include "LegacyAdapters/Data/LegacyDataModels.h"
#include "LegacyAdapters/Data/MasterRepoDataModels.h"

class ItemAdapter
{
public:
    static MRItemDefinition Convert(const LegacyItemRecord& Legacy);
};

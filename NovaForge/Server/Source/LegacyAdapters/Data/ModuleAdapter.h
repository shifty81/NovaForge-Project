#pragma once

#include "LegacyAdapters/Data/LegacyDataModels.h"
#include "LegacyAdapters/Data/MasterRepoDataModels.h"

class ModuleAdapter
{
public:
    static MRModuleDefinition Convert(const LegacyModuleRecord& Legacy);
};

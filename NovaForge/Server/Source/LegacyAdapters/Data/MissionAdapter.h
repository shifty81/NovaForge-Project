#pragma once

#include "LegacyAdapters/Data/LegacyDataModels.h"
#include "LegacyAdapters/Data/MasterRepoDataModels.h"

class MissionAdapter
{
public:
    static MRMissionDefinition Convert(const LegacyMissionRecord& Legacy);
};

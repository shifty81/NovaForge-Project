#pragma once

#include "LegacyAdapters/Data/LegacyDataModels.h"
#include "LegacyAdapters/Data/MasterRepoDataModels.h"

class RecipeAdapter
{
public:
    static MRRecipeDefinition Convert(const LegacyRecipeRecord& Legacy);
};

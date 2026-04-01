#include "LegacyAdapters/Data/RecipeAdapter.h"
#include "LegacyAdapters/Data/DataConversionUtils.h"

MRRecipeDefinition RecipeAdapter::Convert(const LegacyRecipeRecord& Legacy)
{
    MRRecipeDefinition Out;
    Out.Id = DataConversionUtils::MakeId(Legacy.Name);
    Out.Name = Legacy.Name;
    Out.Machine = Legacy.Machine;
    Out.TimeSeconds = Legacy.TimeSeconds;

    for (const auto& Input : Legacy.Inputs)
    {
        Out.Inputs.push_back({DataConversionUtils::MakeId(Input.ItemName), Input.Count});
    }

    return Out;
}

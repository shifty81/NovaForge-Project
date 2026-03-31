#include "LegacyAdapters/Data/ItemAdapter.h"
#include "LegacyAdapters/Data/DataConversionUtils.h"

MRItemDefinition ItemAdapter::Convert(const LegacyItemRecord& Legacy)
{
    MRItemDefinition Out;
    Out.Id = DataConversionUtils::MakeId(Legacy.Name);
    Out.Name = Legacy.Name;
    Out.Category = Legacy.Category;
    Out.Value = Legacy.Value;
    Out.StackSize = Legacy.StackSize;
    return Out;
}

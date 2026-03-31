#include "LegacyAdapters/Data/FactionAdapter.h"
#include "LegacyAdapters/Data/DataConversionUtils.h"

MRFactionDefinition FactionAdapter::Convert(const LegacyFactionRecord& Legacy)
{
    MRFactionDefinition Out;
    Out.Id = DataConversionUtils::MakeId(Legacy.Name);
    Out.Name = Legacy.Name;
    Out.Alignment = Legacy.Alignment;
    return Out;
}

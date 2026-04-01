#include "LegacyAdapters/Data/ModuleAdapter.h"
#include "LegacyAdapters/Data/DataConversionUtils.h"

MRModuleDefinition ModuleAdapter::Convert(const LegacyModuleRecord& Legacy)
{
    MRModuleDefinition Out;
    Out.Id = DataConversionUtils::MakeId(Legacy.Name);
    Out.Name = Legacy.Name;
    Out.Category = Legacy.Category;
    Out.SizeX = Legacy.SizeX;
    Out.SizeY = Legacy.SizeY;
    Out.SizeZ = Legacy.SizeZ;
    return Out;
}

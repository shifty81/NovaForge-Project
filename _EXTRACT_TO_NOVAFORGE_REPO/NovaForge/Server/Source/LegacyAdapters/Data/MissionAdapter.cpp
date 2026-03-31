#include "LegacyAdapters/Data/MissionAdapter.h"
#include "LegacyAdapters/Data/DataConversionUtils.h"

MRMissionDefinition MissionAdapter::Convert(const LegacyMissionRecord& Legacy)
{
    MRMissionDefinition Out;
    Out.Id = DataConversionUtils::MakeId(Legacy.Title);
    Out.Title = Legacy.Title;
    Out.Type = Legacy.Type;
    Out.CreditReward = Legacy.CreditReward;
    return Out;
}

#include "LegacyAdapters/AI/AIBridgeAdapter.h"

MRArbiterTask AIBridgeAdapter::Convert(const LegacyAITask& Legacy)
{
    MRArbiterTask Out;
    Out.Prompt = Legacy.Prompt;

    if (Legacy.Mode == "plan")
    {
        Out.Intent = "design_planning";
        Out.Targets = {"docs", "systems"};
    }
    else if (Legacy.Mode == "code")
    {
        Out.Intent = "code_generation";
        Out.Targets = {"source", "data"};
    }
    else
    {
        Out.Intent = "general_assistance";
        Out.Targets = {"workspace"};
    }

    return Out;
}

#include "LegacyAdapters/Tooling/ToolingBridgeAdapter.h"

MRPanelState ToolingBridgeAdapter::Convert(const LegacyPanelState& Legacy)
{
    MRPanelState Out;
    Out.PanelId = Legacy.PanelType;
    Out.PanelRole = "workspace_panel";
    Out.bVisible = Legacy.bVisible;

    if (Legacy.PanelType == "chat")
    {
        Out.PanelRole = "ai_chat";
    }
    else if (Legacy.PanelType == "repo_map")
    {
        Out.PanelRole = "repo_map";
    }
    else if (Legacy.PanelType == "editor")
    {
        Out.PanelRole = "document_editor";
    }

    return Out;
}

#pragma once

#include <string>

struct LegacyPanelState
{
    std::string PanelType;
    bool bVisible = true;
};

struct MRPanelState
{
    std::string PanelId;
    std::string PanelRole;
    bool bVisible = true;
};

class ToolingBridgeAdapter
{
public:
    static MRPanelState Convert(const LegacyPanelState& Legacy);
};

#include "LegacyAdapters/Tooling/WorkspaceBridgeAdapter.h"

std::vector<std::string> WorkspaceBridgeAdapter::FilterLegacyTabs(const std::vector<std::string>& LegacyTabs)
{
    std::vector<std::string> Out;

    for (const auto& Tab : LegacyTabs)
    {
        if (Tab == "chat" || Tab == "repo_map" || Tab == "editor" || Tab == "diff")
        {
            Out.push_back(Tab);
        }
    }

    return Out;
}

#pragma once

#include <string>
#include <vector>

class WorkspaceBridgeAdapter
{
public:
    static std::vector<std::string> FilterLegacyTabs(const std::vector<std::string>& LegacyTabs);
};

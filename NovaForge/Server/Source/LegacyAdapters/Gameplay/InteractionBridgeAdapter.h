#pragma once

#include <string>

class InteractionBridgeAdapter
{
public:
    static bool IsSafeLegacyInteraction(const std::string& LegacyInteractionType);
};

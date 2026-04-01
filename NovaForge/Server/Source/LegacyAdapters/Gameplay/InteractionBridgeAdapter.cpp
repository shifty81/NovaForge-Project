#include "LegacyAdapters/Gameplay/InteractionBridgeAdapter.h"

bool InteractionBridgeAdapter::IsSafeLegacyInteraction(const std::string& LegacyInteractionType)
{
    return LegacyInteractionType == "inspect"
        || LegacyInteractionType == "pickup"
        || LegacyInteractionType == "use_panel"
        || LegacyInteractionType == "open_container";
}

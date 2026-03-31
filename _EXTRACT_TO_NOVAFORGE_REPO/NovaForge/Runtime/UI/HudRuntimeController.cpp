#include "HudRuntimeController.h"

namespace Runtime::UI
{
void HudRuntimeController::SetMissionText(const std::string& text)
{
m_missionText = text;
// TODO: bind to UI widget tree
}

void HudRuntimeController::SetOxygenRatio(float /*ratio*/)
{
    // TODO: update HUD bar
}

void HudRuntimeController::SetPowerRatio(float /*ratio*/)
{
    // TODO: update HUD bar
}

void HudRuntimeController::ShowInteractionPrompt(const std::string& prompt)
{
    m_interactionPrompt = prompt;
    // TODO: update prompt widget
}

void HudRuntimeController::ClearInteractionPrompt()
{
    m_interactionPrompt.clear();
    // TODO: hide prompt widget
}

void HudRuntimeController::ShowPhaseBanner(const std::string& /*phaseName*/)
{
    // TODO: transient banner animation
}

void HudRuntimeController::ShowMissionCompletePanel()
{
    // TODO: debrief / reward panel
}
}

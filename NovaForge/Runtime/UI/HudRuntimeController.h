#pragma once

#include <string>

namespace Runtime::UI
{
class HudRuntimeController
{
public:
void SetMissionText(const std::string& text);
void SetOxygenRatio(float ratio);
void SetPowerRatio(float ratio);
void ShowInteractionPrompt(const std::string& prompt);
void ClearInteractionPrompt();

    void ShowPhaseBanner(const std::string& phaseName);
    void ShowMissionCompletePanel();

private:
    std::string m_missionText;
    std::string m_interactionPrompt;
};
}

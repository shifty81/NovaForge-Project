// HUDLayer.cpp
// NovaForge UI — runtime HUD layer.

#include "Widgets/HUDLayer.h"

#include <algorithm>

namespace novaforge::ui {

bool HUDLayer::Initialize() { m_visible = true; return true; }
void HUDLayer::Shutdown()   { m_alerts.clear(); m_mission.reset(); }

void HUDLayer::Tick(float delta)
{
    for (auto& a : m_alerts) a.elapsed += delta;
    ClearExpiredAlerts();
}

void HUDLayer::UpdatePlayerStats(const HUDPlayerStats& stats)
{
    m_stats = stats;
}

void HUDLayer::PushAlert(const std::string& text, float durationSeconds)
{
    m_alerts.push_back({ text, durationSeconds, 0.f });
}

void HUDLayer::ClearExpiredAlerts()
{
    m_alerts.erase(
        std::remove_if(m_alerts.begin(), m_alerts.end(),
                       [](const HUDAlertMessage& a){ return a.isExpired(); }),
        m_alerts.end());
}

void HUDLayer::SetActiveMission(const HUDMissionTrack& track)
{
    m_mission = track;
}

void HUDLayer::ClearMission()
{
    m_mission.reset();
}

std::optional<HUDMissionTrack> HUDLayer::GetActiveMission() const
{
    return m_mission;
}

void HUDLayer::SetMissionProgress(float progress)
{
    if (m_mission)
        m_mission->progress = progress;
}

std::vector<UIWidget> HUDLayer::BuildWidgets() const
{
    std::vector<UIWidget> widgets;
    if (!m_visible) return widgets;

    // Health bar
    UIWidget health;
    health.widgetId = "hud_health";
    health.type     = EWidgetType::ProgressBar;
    health.label    = "Health";
    health.progress = (m_stats.maxHealth > 0.f)
        ? (m_stats.health / m_stats.maxHealth) : 0.f;
    health.rect     = { 0.01f, 0.90f, 0.15f, 0.02f };
    widgets.push_back(health);

    // Shield bar
    UIWidget shield;
    shield.widgetId = "hud_shield";
    shield.type     = EWidgetType::ProgressBar;
    shield.label    = "Shields";
    shield.progress = (m_stats.maxShields > 0.f)
        ? (m_stats.shields / m_stats.maxShields) : 0.f;
    shield.rect     = { 0.01f, 0.93f, 0.15f, 0.02f };
    widgets.push_back(shield);

    // Credits label
    UIWidget credits;
    credits.widgetId = "hud_credits";
    credits.type     = EWidgetType::Label;
    credits.label    = "CR: " + std::to_string(static_cast<int>(m_stats.credits));
    credits.rect     = { 0.85f, 0.01f, 0.14f, 0.03f };
    widgets.push_back(credits);

    // Mission tracker
    if (m_mission)
    {
        UIWidget mission;
        mission.widgetId = "hud_mission";
        mission.type     = EWidgetType::Panel;
        mission.label    = m_mission->title + ": " + m_mission->objective;
        mission.rect     = { 0.70f, 0.05f, 0.29f, 0.10f };
        widgets.push_back(mission);
    }

    // Alerts
    float alertY = 0.40f;
    for (const auto& a : m_alerts)
    {
        UIWidget alert;
        alert.widgetId = "hud_alert_" + std::to_string((size_t)&a);
        alert.type     = EWidgetType::Label;
        alert.label    = a.text;
        alert.rect     = { 0.35f, alertY, 0.30f, 0.04f };
        widgets.push_back(alert);
        alertY += 0.05f;
    }

    return widgets;
}

} // namespace novaforge::ui

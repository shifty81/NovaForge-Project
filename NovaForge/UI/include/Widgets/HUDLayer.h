// HUDLayer.h
// NovaForge UI — runtime HUD layer: health, speed, credits, mission tracker, alerts.

#pragma once
#include "Widgets/UIWidgetBase.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace novaforge::ui {

// ---- HUD data bindings --------------------------------------------------

struct HUDPlayerStats
{
    float  health        = 100.f;
    float  maxHealth     = 100.f;
    float  shields       = 100.f;
    float  maxShields    = 100.f;
    float  speed         = 0.f;      ///< m/s
    float  credits       = 0.f;
    uint32_t factionRep  = 0;
};

struct HUDAlertMessage
{
    std::string text;
    float       durationSeconds = 3.f;
    float       elapsed         = 0.f;
    bool        isExpired() const { return elapsed >= durationSeconds; }
};

struct HUDMissionTrack
{
    std::string missionId;
    std::string title;
    std::string objective;
    float       progress = 0.f;  ///< 0-1
};

// ---- HUDLayer -----------------------------------------------------------

class HUDLayer
{
public:
    bool Initialize();
    void Shutdown();
    void Tick(float delta);

    // ---- player stats -----------------------------------------------
    void              UpdatePlayerStats(const HUDPlayerStats& stats);
    const HUDPlayerStats& GetStats() const { return m_stats; }

    // ---- alerts -----------------------------------------------------
    void PushAlert(const std::string& text, float durationSeconds = 3.f);
    const std::vector<HUDAlertMessage>& GetAlerts() const { return m_alerts; }
    void ClearExpiredAlerts();

    // ---- mission tracker --------------------------------------------
    void SetActiveMission(const HUDMissionTrack& track);
    void ClearMission();
    std::optional<HUDMissionTrack> GetActiveMission() const;
    void SetMissionProgress(float progress);

    // ---- visibility -------------------------------------------------
    void SetVisible(bool v) { m_visible = v; }
    bool IsVisible()  const { return m_visible; }

    // ---- widget list (for renderer) ---------------------------------
    std::vector<UIWidget> BuildWidgets() const;

private:
    HUDPlayerStats               m_stats;
    std::vector<HUDAlertMessage> m_alerts;
    std::optional<HUDMissionTrack> m_mission;
    bool                         m_visible = true;
};

} // namespace novaforge::ui

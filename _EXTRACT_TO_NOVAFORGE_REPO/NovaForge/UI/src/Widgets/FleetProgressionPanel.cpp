// FleetProgressionPanel.cpp
// NovaForge UI — fleet / ship progression panel.

#include "Widgets/FleetProgressionPanel.h"

namespace novaforge::ui {

bool FleetProgressionPanel::Initialize() { return true; }
void FleetProgressionPanel::Shutdown()   { m_state = {}; m_open = false; }

void FleetProgressionPanel::Open()  { m_open = true; }
void FleetProgressionPanel::Close() { m_open = false; }

void FleetProgressionPanel::SetFleetState(const FleetProgressionState& state)
{
    m_state = state;
}

void FleetProgressionPanel::UpdateShipXP(const std::string& shipId, float xp)
{
    FleetShipEntry* ship = GetMutableShip(shipId);
    if (!ship) return;
    ship->xp = xp;
    // Compute level from XP.
    while (ship->xp >= ship->xpToNextLevel && ship->level < 10)
    {
        ship->xp -= ship->xpToNextLevel;
        ++ship->level;
        ship->xpToNextLevel *= 1.5f;
    }
}

void FleetProgressionPanel::UpdateShipCondition(const std::string& shipId,
                                                  float conditionPct)
{
    FleetShipEntry* ship = GetMutableShip(shipId);
    if (ship) ship->conditionPct = conditionPct;
}

void FleetProgressionPanel::SelectShip(const std::string& shipId)
{
    m_state.selectedShipId = shipId;
}

std::optional<FleetShipEntry> FleetProgressionPanel::GetSelectedShip() const
{
    for (const auto& s : m_state.ships)
        if (s.shipId == m_state.selectedShipId) return s;
    return std::nullopt;
}

std::vector<UIWidget> FleetProgressionPanel::BuildWidgets() const
{
    std::vector<UIWidget> widgets;
    if (!m_open) return widgets;

    UIWidget panel;
    panel.widgetId = "fleet_panel";
    panel.type     = EWidgetType::Panel;
    panel.label    = "Fleet";
    panel.rect     = { 0.05f, 0.05f, 0.90f, 0.90f };
    widgets.push_back(panel);

    // Fleet XP bar
    UIWidget fleetXP;
    fleetXP.widgetId = "fleet_xp";
    fleetXP.type     = EWidgetType::ProgressBar;
    fleetXP.label    = "Fleet Level " + std::to_string(m_state.fleetLevel);
    fleetXP.progress = (m_state.fleetXPToNext > 0.f)
                       ? (m_state.fleetXP / m_state.fleetXPToNext) : 0.f;
    fleetXP.rect     = { 0.08f, 0.10f, 0.60f, 0.03f };
    widgets.push_back(fleetXP);

    // Ship list
    float rowY = 0.17f;
    for (const auto& ship : m_state.ships)
    {
        UIWidget row;
        row.widgetId = "fleet_ship_" + ship.shipId;
        row.type     = EWidgetType::Button;
        row.label    = ship.shipName + " [" + ship.roleTag + "] Lv" +
                       std::to_string(ship.level);
        row.rect     = { 0.08f, rowY, 0.55f, 0.05f };
        row.state    = (ship.shipId == m_state.selectedShipId)
                       ? EWidgetState::Pressed : EWidgetState::Normal;
        row.enabled  = ship.isActive;
        widgets.push_back(row);

        // Ship XP bar
        UIWidget shipXP;
        shipXP.widgetId = "fleet_ship_xp_" + ship.shipId;
        shipXP.type     = EWidgetType::ProgressBar;
        shipXP.progress = (ship.xpToNextLevel > 0.f)
                          ? (ship.xp / ship.xpToNextLevel) : 0.f;
        shipXP.rect     = { 0.65f, rowY, 0.25f, 0.02f };
        widgets.push_back(shipXP);

        rowY += 0.07f;
    }

    return widgets;
}

FleetShipEntry* FleetProgressionPanel::GetMutableShip(const std::string& shipId)
{
    for (auto& s : m_state.ships)
        if (s.shipId == shipId) return &s;
    return nullptr;
}

} // namespace novaforge::ui

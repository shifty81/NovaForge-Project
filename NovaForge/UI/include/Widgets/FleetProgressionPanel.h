// FleetProgressionPanel.h
// NovaForge UI — fleet / ship progression panel: ship list, roles, XP bars, upgrades.

#pragma once
#include "Widgets/UIWidgetBase.h"

#include <optional>
#include <string>
#include <vector>

namespace novaforge::ui {

struct FleetShipEntry
{
    std::string shipId;
    std::string shipName;
    std::string shipClass;   ///< Scout, Fighter, Hauler, Capital, etc.
    std::string roleTag;     ///< Combat, Trade, Patrol, Salvage
    float       xp          = 0.f;
    float       xpToNextLevel = 1000.f;
    uint32_t    level       = 1;
    uint32_t    crewCount   = 0;
    float       conditionPct = 100.f;
    bool        isActive    = true;
};

struct FleetProgressionState
{
    std::vector<FleetShipEntry> ships;
    uint32_t                    fleetLevel    = 1;
    float                       fleetXP       = 0.f;
    float                       fleetXPToNext = 5000.f;
    uint32_t                    totalCrew     = 0;
    std::string                 selectedShipId;
};

class FleetProgressionPanel
{
public:
    bool Initialize();
    void Shutdown();

    // ---- open / close -----------------------------------------------
    void Open();
    void Close();
    bool IsOpen() const { return m_open; }

    // ---- data binding -----------------------------------------------
    void SetFleetState(const FleetProgressionState& state);
    void UpdateShipXP(const std::string& shipId, float xp);
    void UpdateShipCondition(const std::string& shipId, float conditionPct);

    // ---- interaction ------------------------------------------------
    void SelectShip(const std::string& shipId);
    std::optional<FleetShipEntry> GetSelectedShip() const;

    // ---- widget export ----------------------------------------------
    std::vector<UIWidget> BuildWidgets() const;

    const FleetProgressionState& GetState() const { return m_state; }

private:
    FleetProgressionState m_state;
    bool                  m_open = false;

    FleetShipEntry* GetMutableShip(const std::string& shipId);
};

} // namespace novaforge::ui

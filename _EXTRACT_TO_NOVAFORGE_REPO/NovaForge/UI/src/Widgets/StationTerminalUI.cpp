// StationTerminalUI.cpp
// NovaForge UI — station terminal screen.

#include "Widgets/StationTerminalUI.h"

namespace novaforge::ui {

bool StationTerminalUI::Initialize() { return true; }
void StationTerminalUI::Shutdown()
{
    m_storageRows.clear();
    m_mfgRows.clear();
    m_open = false;
}

void StationTerminalUI::Open(uint64_t stationId)
{
    m_stationId = stationId;
    m_open      = true;
    m_activeTab = EStationTab::Storage;
}

void StationTerminalUI::Close() { m_open = false; }

void StationTerminalUI::SetStorageRows(const std::vector<StationStorageRow>& rows)
{
    m_storageRows = rows;
}

void StationTerminalUI::SetManufacturingRows(
    const std::vector<StationManufacturingRow>& rows)
{
    m_mfgRows = rows;
}

std::vector<UIWidget> StationTerminalUI::BuildWidgets() const
{
    std::vector<UIWidget> widgets;
    if (!m_open) return widgets;

    // Background panel
    UIWidget bg;
    bg.widgetId = "st_bg";
    bg.type     = EWidgetType::Panel;
    bg.label    = "Station Terminal";
    bg.rect     = { 0.05f, 0.05f, 0.90f, 0.90f };
    widgets.push_back(bg);

    // Tab buttons
    const char* tabNames[] = { "Storage","Manufacturing","Repair","Resupply","Market" };
    for (int i = 0; i < 5; ++i)
    {
        UIWidget tab;
        tab.widgetId = std::string("st_tab_") + tabNames[i];
        tab.type     = EWidgetType::Button;
        tab.label    = tabNames[i];
        tab.rect     = { 0.07f + i * 0.17f, 0.08f, 0.15f, 0.04f };
        tab.state    = (static_cast<int>(m_activeTab) == i)
                       ? EWidgetState::Pressed : EWidgetState::Normal;
        widgets.push_back(tab);
    }

    if (m_activeTab == EStationTab::Storage)
    {
        float rowY = 0.16f;
        for (const auto& row : m_storageRows)
        {
            UIWidget r;
            r.widgetId = "st_stor_" + row.itemId;
            r.type     = EWidgetType::Slot;
            r.label    = row.displayName + " x" + std::to_string(row.quantity);
            r.rect     = { 0.07f, rowY, 0.86f, 0.04f };
            widgets.push_back(r);
            rowY += 0.05f;
        }
    }
    else if (m_activeTab == EStationTab::Manufacturing)
    {
        float rowY = 0.16f;
        for (const auto& job : m_mfgRows)
        {
            UIWidget r;
            r.widgetId  = "st_mfg_" + job.jobId;
            r.type      = EWidgetType::ProgressBar;
            r.label     = job.outputItemName + " — " + job.statusLabel;
            r.progress  = job.progressPct / 100.f;
            r.rect      = { 0.07f, rowY, 0.86f, 0.04f };
            widgets.push_back(r);
            rowY += 0.05f;
        }
    }
    else if (m_activeTab == EStationTab::Repair)
    {
        UIWidget info;
        info.widgetId = "st_repair_info";
        info.type     = EWidgetType::Label;
        info.label    = "Repair cost: " + std::to_string(static_cast<int>(m_repairCost)) + " CR";
        info.rect     = { 0.07f, 0.20f, 0.50f, 0.04f };
        widgets.push_back(info);

        UIWidget btn;
        btn.widgetId = "st_repair_btn";
        btn.type     = EWidgetType::Button;
        btn.label    = "Repair Ship";
        btn.rect     = { 0.07f, 0.26f, 0.20f, 0.05f };
        widgets.push_back(btn);
    }
    else if (m_activeTab == EStationTab::Resupply)
    {
        UIWidget info;
        info.widgetId = "st_resupply_info";
        info.type     = EWidgetType::Label;
        info.label    = "Resupply cost: " + std::to_string(static_cast<int>(m_resupplyCost)) + " CR";
        info.rect     = { 0.07f, 0.20f, 0.50f, 0.04f };
        widgets.push_back(info);

        UIWidget btn;
        btn.widgetId = "st_resupply_btn";
        btn.type     = EWidgetType::Button;
        btn.label    = "Resupply";
        btn.rect     = { 0.07f, 0.26f, 0.20f, 0.05f };
        widgets.push_back(btn);
    }

    return widgets;
}

} // namespace novaforge::ui

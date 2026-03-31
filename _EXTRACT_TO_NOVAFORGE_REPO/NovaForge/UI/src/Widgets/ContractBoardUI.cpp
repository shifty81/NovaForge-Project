// ContractBoardUI.cpp
// NovaForge UI — contract board screen.

#include "Widgets/ContractBoardUI.h"

#include <algorithm>

namespace novaforge::ui {

bool ContractBoardUI::Initialize() { return true; }
void ContractBoardUI::Shutdown()   { m_contracts.clear(); }

void ContractBoardUI::SetContracts(const std::vector<ContractBoardEntry>& contracts)
{
    m_contracts = contracts;
}

void ContractBoardUI::MarkContractLocked(const std::string& contractId, bool locked)
{
    ContractBoardEntry* e = GetMutable(contractId);
    if (e) e->isAvailable = !locked;
}

void ContractBoardUI::Open()  { m_open = true; }
void ContractBoardUI::Close() { m_open = false; }

void ContractBoardUI::SelectContract(const std::string& contractId)
{
    m_selectedId = contractId;
    for (auto& c : m_contracts)
        c.isSelected = (c.contractId == contractId);
}

std::optional<ContractBoardEntry> ContractBoardUI::GetSelected() const
{
    for (const auto& c : m_contracts)
        if (c.contractId == m_selectedId) return c;
    return std::nullopt;
}

void ContractBoardUI::AcceptSelected()
{
    if (!m_selectedId.empty() && m_acceptCb)
        m_acceptCb(m_selectedId);
}

void ContractBoardUI::DeclineSelected()
{
    m_selectedId.clear();
    for (auto& c : m_contracts) c.isSelected = false;
}

std::vector<UIWidget> ContractBoardUI::BuildWidgets() const
{
    std::vector<UIWidget> widgets;
    if (!m_open) return widgets;

    UIWidget panel;
    panel.widgetId = "cb_panel";
    panel.type     = EWidgetType::Panel;
    panel.label    = "Contract Board";
    panel.rect     = { 0.10f, 0.10f, 0.80f, 0.80f };
    widgets.push_back(panel);

    float rowY = 0.18f;
    for (const auto& c : m_contracts)
    {
        UIWidget row;
        row.widgetId = "cb_row_" + c.contractId;
        row.type     = EWidgetType::Button;
        row.label    = c.title + "  [" + std::to_string(static_cast<int>(c.creditReward)) + " CR]";
        row.rect     = { 0.12f, rowY, 0.76f, 0.05f };
        row.enabled  = c.isAvailable;
        row.state    = c.isSelected ? EWidgetState::Pressed : EWidgetState::Normal;
        widgets.push_back(row);
        rowY += 0.06f;
    }

    // Accept button
    UIWidget accept;
    accept.widgetId = "cb_accept";
    accept.type     = EWidgetType::Button;
    accept.label    = "Accept";
    accept.rect     = { 0.60f, 0.84f, 0.14f, 0.04f };
    accept.enabled  = !m_selectedId.empty();
    widgets.push_back(accept);

    return widgets;
}

ContractBoardEntry* ContractBoardUI::GetMutable(const std::string& id)
{
    for (auto& c : m_contracts)
        if (c.contractId == id) return &c;
    return nullptr;
}

} // namespace novaforge::ui

// InventoryScreen.cpp
// NovaForge UI — inventory screen.

#include "Widgets/InventoryScreen.h"

namespace novaforge::ui {

bool InventoryScreen::Initialize() { return true; }
void InventoryScreen::Shutdown()   { m_state = {}; }

void InventoryScreen::SetItems(const std::vector<InventoryScreenItem>& items)
{
    m_state.items = items;
}

void InventoryScreen::SetCapacity(float currentMass, float maxMass)
{
    m_state.currentMass = currentMass;
    m_state.maxMass     = maxMass;
}

void InventoryScreen::Open()  { m_state.isOpen = true; }
void InventoryScreen::Close() { m_state.isOpen = false; }

void InventoryScreen::SelectItem(const std::string& itemId)
{
    m_state.selectedItemId = itemId;
}

void InventoryScreen::ClearSelection()
{
    m_state.selectedItemId.clear();
}

std::optional<InventoryScreenItem> InventoryScreen::GetSelectedItem() const
{
    for (const auto& item : m_state.items)
        if (item.itemId == m_state.selectedItemId) return item;
    return std::nullopt;
}

void InventoryScreen::SetCategoryFilter(const std::string& category)
{
    m_categoryFilter = category;
}

void InventoryScreen::ClearFilter()
{
    m_categoryFilter.clear();
}

std::vector<InventoryScreenItem> InventoryScreen::GetFiltered() const
{
    if (m_categoryFilter.empty()) return m_state.items;
    std::vector<InventoryScreenItem> result;
    for (const auto& item : m_state.items)
        if (item.categoryTag == m_categoryFilter)
            result.push_back(item);
    return result;
}

std::vector<UIWidget> InventoryScreen::BuildWidgets() const
{
    std::vector<UIWidget> widgets;
    if (!m_state.isOpen) return widgets;

    // Background panel
    UIWidget bg;
    bg.widgetId = "inv_bg";
    bg.type     = EWidgetType::Panel;
    bg.rect     = { 0.10f, 0.10f, 0.80f, 0.80f };
    widgets.push_back(bg);

    // Capacity label
    UIWidget cap;
    cap.widgetId = "inv_capacity";
    cap.type     = EWidgetType::Label;
    cap.label    = "Mass: " + std::to_string(static_cast<int>(m_state.currentMass)) +
                   " / " + std::to_string(static_cast<int>(m_state.maxMass)) + " kg";
    cap.rect     = { 0.12f, 0.12f, 0.40f, 0.03f };
    widgets.push_back(cap);

    // Item slots
    auto filtered = GetFiltered();
    float slotX = 0.12f, slotY = 0.17f;
    for (const auto& item : filtered)
    {
        UIWidget slot;
        slot.widgetId = "inv_slot_" + item.itemId;
        slot.type     = EWidgetType::Slot;
        slot.label    = item.displayName + " x" + std::to_string(item.quantity);
        slot.iconId   = item.iconId;
        slot.rect     = { slotX, slotY, 0.08f, 0.08f };
        slot.state    = (item.itemId == m_state.selectedItemId)
                        ? EWidgetState::Pressed : EWidgetState::Normal;
        widgets.push_back(slot);
        slotX += 0.09f;
        if (slotX > 0.82f) { slotX = 0.12f; slotY += 0.09f; }
    }

    return widgets;
}

} // namespace novaforge::ui

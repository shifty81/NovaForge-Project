// InventoryScreen.h
// NovaForge UI — inventory screen: item grid, item details, equipment slots.

#pragma once
#include "Widgets/UIWidgetBase.h"

#include <optional>
#include <string>
#include <vector>

namespace novaforge::ui {

struct InventoryScreenItem
{
    std::string itemId;
    std::string displayName;
    uint32_t    quantity    = 0;
    std::string iconId;
    std::string categoryTag;
    float       massPerUnit = 0.f;
    float       baseValue   = 0.f;
};

struct InventoryScreenState
{
    std::vector<InventoryScreenItem> items;
    float                            currentMass  = 0.f;
    float                            maxMass      = 0.f;
    std::string                      selectedItemId;
    bool                             isOpen       = false;
};

class InventoryScreen
{
public:
    bool Initialize();
    void Shutdown();

    // ---- data binding -----------------------------------------------
    void SetItems(const std::vector<InventoryScreenItem>& items);
    void SetCapacity(float currentMass, float maxMass);

    // ---- interaction ------------------------------------------------
    void Open();
    void Close();
    bool IsOpen() const { return m_state.isOpen; }

    void SelectItem(const std::string& itemId);
    void ClearSelection();
    std::optional<InventoryScreenItem> GetSelectedItem() const;

    // ---- filter / sort ----------------------------------------------
    void SetCategoryFilter(const std::string& category);
    void ClearFilter();

    // ---- widget export for renderer ---------------------------------
    std::vector<UIWidget> BuildWidgets() const;

    const InventoryScreenState& GetState() const { return m_state; }

private:
    InventoryScreenState m_state;
    std::string          m_categoryFilter;

    std::vector<InventoryScreenItem> GetFiltered() const;
};

} // namespace novaforge::ui

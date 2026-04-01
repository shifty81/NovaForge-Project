#pragma once

#include "Gameplay/GameplayTypes.h"
#include <string>
#include <vector>

class DataRegistry;

class InventorySystem
{
public:
    bool Initialize(DataRegistry& InDataRegistry);

    bool AddItem(const std::string& ItemId, int Count);
    bool RemoveItem(const std::string& ItemId, int Count);
    bool HasItem(const std::string& ItemId, int Count) const;

    const std::vector<InventorySlot>& GetSlots() const;
    void LogInventory() const;

private:
    DataRegistry* Data = nullptr;
    std::vector<InventorySlot> Slots;
};

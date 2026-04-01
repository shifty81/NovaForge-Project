#include "Gameplay/InventorySystem.h"
#include "Data/DataRegistry.h"
#include <iostream>

bool InventorySystem::Initialize(DataRegistry& InDataRegistry)
{
    Data = &InDataRegistry;
    std::cout << "[InventorySystem] Initialize\n";
    return true;
}

bool InventorySystem::AddItem(const std::string& ItemId, int Count)
{
    if (!Data || !Data->FindItemDefinition(ItemId) || Count <= 0)
    {
        return false;
    }

    for (auto& Slot : Slots)
    {
        if (Slot.ItemId == ItemId)
        {
            Slot.Count += Count;
            return true;
        }
    }

    Slots.push_back({ItemId, Count});
    return true;
}

bool InventorySystem::RemoveItem(const std::string& ItemId, int Count)
{
    if (Count <= 0)
    {
        return false;
    }

    for (auto It = Slots.begin(); It != Slots.end(); ++It)
    {
        if (It->ItemId == ItemId && It->Count >= Count)
        {
            It->Count -= Count;
            if (It->Count == 0)
            {
                Slots.erase(It);
            }
            return true;
        }
    }

    return false;
}

bool InventorySystem::HasItem(const std::string& ItemId, int Count) const
{
    for (const auto& Slot : Slots)
    {
        if (Slot.ItemId == ItemId && Slot.Count >= Count)
        {
            return true;
        }
    }

    return false;
}

const std::vector<InventorySlot>& InventorySystem::GetSlots() const
{
    return Slots;
}

void InventorySystem::LogInventory() const
{
    std::cout << "[InventorySystem] Inventory Slots=" << Slots.size() << "\n";
    for (const auto& Slot : Slots)
    {
        std::cout << "  - " << Slot.ItemId << " x" << Slot.Count << "\n";
    }
}

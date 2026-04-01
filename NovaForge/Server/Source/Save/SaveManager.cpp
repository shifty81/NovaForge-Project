#include "Save/SaveManager.h"
#include <iostream>

bool SaveManager::Initialize()
{
    std::cout << "[SaveManager] Initialize\n";
    return true;
}

bool SaveManager::Save(const std::string& SlotName)
{
    for (auto& Slot : Slots)
    {
        if (Slot.SlotName == SlotName)
        {
            Slot.bExists = true;
            std::cout << "[SaveManager] Saved existing slot " << SlotName << "\n";
            return true;
        }
    }

    Slots.push_back({SlotName, true});
    std::cout << "[SaveManager] Saved new slot " << SlotName << "\n";
    return true;
}

bool SaveManager::Load(const std::string& SlotName)
{
    for (const auto& Slot : Slots)
    {
        if (Slot.SlotName == SlotName && Slot.bExists)
        {
            std::cout << "[SaveManager] Loaded slot " << SlotName << "\n";
            return true;
        }
    }

    std::cout << "[SaveManager] Failed to load slot " << SlotName << "\n";
    return false;
}

void SaveManager::Shutdown()
{
    std::cout << "[SaveManager] Shutdown\n";
}

const std::vector<SaveSlotInfo>& SaveManager::GetSlots() const
{
    return Slots;
}

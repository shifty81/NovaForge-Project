#include "Characters/Equipment/EquipmentSystem.h"
#include <iostream>

bool EquipmentSystem::Initialize()
{
    std::cout << "[EquipmentSystem] Initialize\n";
    return true;
}

void EquipmentSystem::RegisterDefinition(const EquipmentDefinition& Definition)
{
    Definitions.push_back(Definition);
    std::cout << "[EquipmentSystem] Registered " << Definition.EquipmentId << "\n";
}

bool EquipmentSystem::EquipItem(const std::string& CharacterId, const EquippedItem& Item)
{
    EquippedItems.push_back(Item);
    std::cout << "[EquipmentSystem] Equipped " << Item.EquipmentId
              << " on " << CharacterId
              << " socket=" << Item.SocketId << "\n";
    return true;
}

const EquipmentDefinition* EquipmentSystem::FindDefinition(const std::string& EquipmentId) const
{
    for (const auto& Definition : Definitions)
    {
        if (Definition.EquipmentId == EquipmentId)
        {
            return &Definition;
        }
    }
    return nullptr;
}

void EquipmentSystem::Tick(float)
{
    std::cout << "[EquipmentSystem] Definitions=" << Definitions.size()
              << " Equipped=" << EquippedItems.size() << "\n";
}

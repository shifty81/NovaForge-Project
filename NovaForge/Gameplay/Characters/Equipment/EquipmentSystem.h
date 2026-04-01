#pragma once

#include "Characters/Equipment/EquipmentTypes.h"
#include <string>
#include <vector>

class EquipmentSystem
{
public:
    bool Initialize();
    void RegisterDefinition(const EquipmentDefinition& Definition);
    bool EquipItem(const std::string& CharacterId, const EquippedItem& Item);
    const EquipmentDefinition* FindDefinition(const std::string& EquipmentId) const;
    void Tick(float DeltaTime);

private:
    std::vector<EquipmentDefinition> Definitions;
    std::vector<EquippedItem> EquippedItems;
};

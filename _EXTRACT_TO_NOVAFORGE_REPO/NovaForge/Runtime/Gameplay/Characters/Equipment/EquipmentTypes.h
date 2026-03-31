#pragma once

#include <string>

struct EquipmentDefinition
{
    std::string EquipmentId;
    std::string Category;
    std::string MeshId;
    std::string SocketId;
    std::string UpgradeTag;
};

struct EquippedItem
{
    std::string EquipmentId;
    std::string SocketId;
    bool bActive = true;
};

#pragma once

#include <string>

enum class EExteriorTargetType
{
    RepairPoint,
    SalvageNode,
    TetherAnchor,
    ServicePanel
};

struct ExteriorTarget
{
    std::string Id;
    EExteriorTargetType Type = EExteriorTargetType::RepairPoint;
    bool bInteractable = true;
};

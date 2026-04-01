#pragma once

#include "Gameplay/Interaction/ExteriorTargetTypes.h"
#include <vector>
#include <string>

class ExteriorInteractionSystem
{
public:
    bool Initialize();
    void RegisterTarget(const ExteriorTarget& Target);
    bool Interact(const std::string& TargetId);

private:
    std::vector<ExteriorTarget> Targets;
};

#include "Gameplay/Interaction/ExteriorInteractionSystem.h"
#include <iostream>

bool ExteriorInteractionSystem::Initialize()
{
    std::cout << "[ExteriorInteraction] Initialize\n";
    return true;
}

void ExteriorInteractionSystem::RegisterTarget(const ExteriorTarget& Target)
{
    Targets.push_back(Target);
}

bool ExteriorInteractionSystem::Interact(const std::string& TargetId)
{
    for (const auto& Target : Targets)
    {
        if (Target.Id == TargetId && Target.bInteractable)
        {
            std::cout << "[ExteriorInteraction] Interacted with " << TargetId
                      << " Type=" << static_cast<int>(Target.Type) << "\n";
            return true;
        }
    }
    return false;
}

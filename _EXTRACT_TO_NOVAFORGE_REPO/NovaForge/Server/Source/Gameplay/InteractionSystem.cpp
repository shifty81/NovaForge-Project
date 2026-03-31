#include "Gameplay/InteractionSystem.h"
#include <iostream>

bool InteractionSystem::Initialize()
{
    std::cout << "[InteractionSystem] Initialize\n";
    return true;
}

bool InteractionSystem::TryInteract(const std::string& TargetType, const std::string& TargetId)
{
    std::cout << "[InteractionSystem] Interact TargetType=" << TargetType
              << " TargetId=" << TargetId << "\n";
    return true;
}

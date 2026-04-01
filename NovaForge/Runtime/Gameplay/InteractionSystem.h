#pragma once

#include <string>

class InteractionSystem
{
public:
    bool Initialize();
    bool TryInteract(const std::string& TargetType, const std::string& TargetId);
};

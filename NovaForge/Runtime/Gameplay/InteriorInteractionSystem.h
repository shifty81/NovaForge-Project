#pragma once

#include <string>

class ShipInteriorShell;

class InteriorInteractionSystem
{
public:
    bool Initialize(ShipInteriorShell& InShell);
    void Interact(const std::string& TargetId);

private:
    ShipInteriorShell* Shell = nullptr;
};

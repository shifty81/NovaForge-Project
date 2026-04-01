#include "Gameplay/InteriorInteractionSystem.h"
#include "Gameplay/ShipInteriorShell.h"

bool InteriorInteractionSystem::Initialize(ShipInteriorShell& InShell)
{
    Shell = &InShell;
    return true;
}

void InteriorInteractionSystem::Interact(const std::string& TargetId)
{
    if (!Shell) return;

    if (TargetId == "door_airlock_inner")
    {
        Shell->ToggleDoor();
    }
    else if (TargetId == "reactor_panel_01")
    {
        Shell->InspectReactor();
    }
    else if (TargetId == "container_utility_01")
    {
        Shell->InspectContainer();
    }
    else if (TargetId == "airlock_01")
    {
        Shell->CycleAirlock();
    }
}

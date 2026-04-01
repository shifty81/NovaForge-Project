#include "Gameplay/ShipInteriorShell.h"
#include <iostream>

bool ShipInteriorShell::Initialize()
{
    std::cout << "[ShipInteriorShell] Initialize room=" << Room.RoomId << "\n";
    return true;
}

void ShipInteriorShell::Tick(float)
{
    std::cout << "[ShipInteriorShell] Room Oxygen=" << (Room.bHasOxygen ? "Yes" : "No")
              << " Gravity=" << (Room.bHasGravity ? "Yes" : "No")
              << " Pressure=" << (Room.bPressurized ? "Yes" : "No")
              << "\n";
}

const RoomEnvironmentState& ShipInteriorShell::GetRoomState() const { return Room; }
const DoorModuleState& ShipInteriorShell::GetDoorState() const { return Door; }
const ContainerModuleState& ShipInteriorShell::GetContainerState() const { return Container; }
const ReactorPanelState& ShipInteriorShell::GetReactorState() const { return Reactor; }
const AirlockState& ShipInteriorShell::GetAirlockState() const { return Airlock; }

void ShipInteriorShell::ToggleDoor()
{
    if (!Door.bLocked)
    {
        Door.bOpen = !Door.bOpen;
        std::cout << "[ShipInteriorShell] Door " << (Door.bOpen ? "Opened" : "Closed") << "\n";
    }
}

void ShipInteriorShell::CycleAirlock()
{
    switch (Airlock.State)
    {
        case EAirlockState::IdleClosed:
            Airlock.State = EAirlockState::CyclingToExterior;
            Room.bPressurized = false;
            break;
        case EAirlockState::CyclingToExterior:
            Airlock.State = EAirlockState::ExteriorReady;
            Airlock.bOuterDoorOpen = true;
            break;
        case EAirlockState::ExteriorReady:
            Airlock.State = EAirlockState::CyclingToInterior;
            Airlock.bOuterDoorOpen = false;
            break;
        case EAirlockState::CyclingToInterior:
            Airlock.State = EAirlockState::IdleClosed;
            Room.bPressurized = true;
            break;
    }

    std::cout << "[ShipInteriorShell] Airlock state changed\n";
}

void ShipInteriorShell::InspectReactor()
{
    std::cout << "[ShipInteriorShell] Reactor Powered=" << (Reactor.bPowered ? "Yes" : "No")
              << " Heat=" << Reactor.HeatLevel << "\n";
}

void ShipInteriorShell::InspectContainer() const
{
    std::cout << "[ShipInteriorShell] Container Slots=" << Container.SlotCount << "\n";
}

#pragma once

#include "Gameplay/ShipInteriorTypes.h"
#include <string>
#include <vector>

class ShipInteriorShell
{
public:
    bool Initialize();
    void Tick(float DeltaTime);

    const RoomEnvironmentState& GetRoomState() const;
    const DoorModuleState& GetDoorState() const;
    const ContainerModuleState& GetContainerState() const;
    const ReactorPanelState& GetReactorState() const;
    const AirlockState& GetAirlockState() const;

    void ToggleDoor();
    void CycleAirlock();
    void InspectReactor();
    void InspectContainer() const;

private:
    RoomEnvironmentState Room{"room_main", true, true, true};
    DoorModuleState Door{"door_airlock_inner", false, false};
    ContainerModuleState Container{"container_utility_01", 16};
    ReactorPanelState Reactor{"reactor_panel_01", true, 0.15f};
    AirlockState Airlock{"airlock_01", EAirlockState::IdleClosed, false, false};
};

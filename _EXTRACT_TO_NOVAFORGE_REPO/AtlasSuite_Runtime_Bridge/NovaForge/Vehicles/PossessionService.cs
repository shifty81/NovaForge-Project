using System.Collections.Generic;

namespace Runtime.NovaForge.Vehicles;

public sealed class PossessionService : IPossessionService
{
    private readonly Dictionary<string, PossessionState> _states = new();

    public PossessionState GetCurrentState(string playerId)
    {
        if (!_states.TryGetValue(playerId, out var state))
        {
            state = new PossessionState
            {
                PlayerId = playerId,
                ControlledEntityId = playerId,
                ControlledEntityType = "rig",
                CameraProfile = "rig_default",
                InputProfile = "rig_default",
                LastSafeRoomOrZone = "dev_world_spawn"
            };
            _states[playerId] = state;
        }

        return state;
    }

    public bool TryPossessMech(string playerId, string mechId)
    {
        var state = GetCurrentState(playerId);
        state.ControlledEntityId = mechId;
        state.ControlledEntityType = "mech";
        state.CameraProfile = "mech_default";
        state.InputProfile = "mech_default";
        return true;
    }

    public bool TryPossessShip(string playerId, string shipId, string entryPointId)
    {
        var state = GetCurrentState(playerId);
        state.ControlledEntityId = shipId;
        state.ControlledEntityType = "ship";
        state.CameraProfile = "ship_pilot_default";
        state.InputProfile = "ship_pilot_default";
        state.LastSafeRoomOrZone = entryPointId;
        return true;
    }

    public bool TryReturnToRig(string playerId)
    {
        var state = GetCurrentState(playerId);
        state.ControlledEntityId = playerId;
        state.ControlledEntityType = "rig";
        state.CameraProfile = "rig_default";
        state.InputProfile = "rig_default";
        return true;
    }
}

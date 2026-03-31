using Runtime.NovaForge.Vehicles;

namespace Runtime.NovaForge.DevWorld.Services;

public sealed class VehicleSmokeTestService
{
    public string Run(string playerId, IPossessionService possessionService)
    {
        var start = possessionService.GetCurrentState(playerId).ControlledEntityType;
        possessionService.TryPossessMech(playerId, "dev_mech_mk1");
        var mech = possessionService.GetCurrentState(playerId).ControlledEntityType;
        possessionService.TryReturnToRig(playerId);
        possessionService.TryPossessShip(playerId, "dev_ship_starter", "cockpit_main");
        var ship = possessionService.GetCurrentState(playerId).ControlledEntityType;
        possessionService.TryReturnToRig(playerId);
        var end = possessionService.GetCurrentState(playerId).ControlledEntityType;
        return $"start={start}; mech={mech}; ship={ship}; end={end}";
    }
}

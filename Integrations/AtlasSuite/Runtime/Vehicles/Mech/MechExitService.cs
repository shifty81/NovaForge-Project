namespace Runtime.NovaForge.Vehicles.Mech;

public sealed class MechExitService
{
    public bool TryExit(string playerId, Runtime.NovaForge.Vehicles.IPossessionService possessionService)
    {
        return possessionService.TryReturnToRig(playerId);
    }
}

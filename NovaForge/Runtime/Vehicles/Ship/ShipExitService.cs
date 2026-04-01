namespace Runtime.NovaForge.Vehicles.Ship;

public sealed class ShipExitService
{
    public bool TryExit(string playerId, Runtime.NovaForge.Vehicles.IPossessionService possessionService)
    {
        return possessionService.TryReturnToRig(playerId);
    }
}

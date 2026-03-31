namespace Runtime.NovaForge.Vehicles.Ship;

public sealed class ShipCockpitInteractable
{
    public string ShipId { get; set; } = "dev_ship_starter";
    public string EntryPointId { get; set; } = "cockpit_main";

    public bool TryInteract(string playerId, Runtime.NovaForge.Vehicles.IPossessionService possessionService)
    {
        return possessionService.TryPossessShip(playerId, ShipId, EntryPointId);
    }
}

namespace Runtime.NovaForge.Vehicles.Mech;

public sealed class MechEntryInteractable
{
    public string MechId { get; set; } = "dev_mech_mk1";

    public bool TryInteract(string playerId, Runtime.NovaForge.Vehicles.IPossessionService possessionService)
    {
        return possessionService.TryPossessMech(playerId, MechId);
    }
}

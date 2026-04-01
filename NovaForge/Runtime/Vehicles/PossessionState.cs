namespace Runtime.NovaForge.Vehicles;

public sealed class PossessionState
{
    public string PlayerId { get; set; } = string.Empty;
    public string ControlledEntityId { get; set; } = string.Empty;
    public string ControlledEntityType { get; set; } = "rig"; // rig | mech | ship
    public string CameraProfile { get; set; } = string.Empty;
    public string InputProfile { get; set; } = string.Empty;
    public string LastSafeRoomOrZone { get; set; } = string.Empty;
}

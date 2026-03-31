namespace Runtime.NovaForge.Vehicles.Ship;

public sealed class ConstructControlSnapshot
{
    public string ConstructId { get; set; } = string.Empty;
    public string ConstructClass { get; set; } = string.Empty;
    public string CargoContainerRef { get; set; } = string.Empty;
    public string[] ModuleRefs { get; set; } = System.Array.Empty<string>();
}

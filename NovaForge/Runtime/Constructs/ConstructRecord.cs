namespace Runtime.NovaForge.Constructs;

public sealed class ConstructRecord
{
    public string ConstructId { get; set; } = string.Empty;
    public string ConstructClass { get; set; } = string.Empty; // ship | mech
    public string DisplayName { get; set; } = string.Empty;
    public string CargoContainerRef { get; set; } = string.Empty;
    public string[] ModuleRefs { get; set; } = System.Array.Empty<string>();
}

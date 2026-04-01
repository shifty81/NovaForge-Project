namespace Runtime.NovaForge.Builder.Salvage;

public sealed class SalvageTargetRecord
{
    public string ConstructId { get; set; } = string.Empty;
    public string PlacementId { get; set; } = string.Empty;
    public string PartId { get; set; } = string.Empty;
    public SalvageMarkState State { get; set; } = SalvageMarkState.None;
}

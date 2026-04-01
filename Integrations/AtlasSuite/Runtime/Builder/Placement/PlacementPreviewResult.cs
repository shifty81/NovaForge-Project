namespace Runtime.NovaForge.Builder.Placement;

public sealed class PlacementPreviewResult
{
    public bool IsValid { get; set; }
    public string Reason { get; set; } = string.Empty;
    public BuilderPlacementRecord? PreviewRecord { get; set; }
}

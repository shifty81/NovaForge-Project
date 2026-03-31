namespace Runtime.NovaForge.Builder.Placement;

public sealed class BuilderPlacementRecord
{
    public string PlacementId { get; set; } = string.Empty;
    public string ConstructId { get; set; } = string.Empty;
    public string PartId { get; set; } = string.Empty;
    public string SnapProfileId { get; set; } = string.Empty;
    public int GridX { get; set; }
    public int GridY { get; set; }
    public int GridZ { get; set; }
    public int Rotation { get; set; }
    public float Durability { get; set; }
    public PlacementState State { get; set; } = PlacementState.Ghost;
}

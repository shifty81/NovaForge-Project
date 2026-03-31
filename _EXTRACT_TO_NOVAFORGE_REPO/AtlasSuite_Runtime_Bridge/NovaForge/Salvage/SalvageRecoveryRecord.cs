using System.Collections.Generic;

namespace Runtime.NovaForge.Salvage;

public sealed class SalvageRecoveryRecord
{
    public string ConstructId { get; set; } = string.Empty;
    public string PlacementId { get; set; } = string.Empty;
    public string RecoveryTableId { get; set; } = string.Empty;
    public List<string> OutputItemIds { get; set; } = new();
}

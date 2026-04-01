namespace Runtime.NovaForge.Repair;

public sealed record RepairActionDef(
    string Id,
    string ToolRequired,
    float TimeSec);

namespace Runtime.NovaForge.Hazards.Breach;

public sealed record BreachState(
    string TargetId,
    string ZoneId,
    bool IsActive,
    float Severity,
    bool IsSealed);

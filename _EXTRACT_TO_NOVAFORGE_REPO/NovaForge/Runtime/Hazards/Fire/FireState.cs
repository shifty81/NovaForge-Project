namespace Runtime.NovaForge.Hazards.Fire;

public sealed record FireState(
    string TargetId,
    string ZoneId,
    bool IsActive,
    float Intensity,
    float Heat);

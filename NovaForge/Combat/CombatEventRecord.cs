namespace Runtime.NovaForge.Combat;

public sealed record CombatEventRecord(
    string SourceId,
    string TargetId,
    string ZoneId,
    string DamageType,
    float AppliedDamage,
    bool CausedBreach,
    bool IgnitedFire);

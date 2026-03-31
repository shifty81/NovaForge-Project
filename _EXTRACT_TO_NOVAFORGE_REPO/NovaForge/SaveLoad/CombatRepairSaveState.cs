namespace Runtime.NovaForge.SaveLoad;

public sealed record CombatRepairSaveState(
    string TargetId,
    string ZoneId,
    bool HasActiveBreach,
    bool HasActiveFire,
    float LastAppliedDamage);

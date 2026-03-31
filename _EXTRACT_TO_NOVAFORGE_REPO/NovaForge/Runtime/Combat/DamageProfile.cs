namespace Runtime.NovaForge.Combat;

public sealed record DamageProfile(
    string Id,
    string DamageType,
    float BaseDamage,
    float Penetration,
    float StructureMultiplier,
    float ModuleMultiplier);

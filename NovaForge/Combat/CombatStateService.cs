using System.Collections.Generic;

namespace Runtime.NovaForge.Combat;

public sealed class CombatStateService : ICombatStateService
{
    private readonly List<CombatEventRecord> _events = new();

    public IReadOnlyList<CombatEventRecord> GetRecentEvents() => _events;

    public CombatEventRecord ApplyDamage(string sourceId, string targetId, string zoneId, DamageProfile profile)
    {
        var breach = profile.DamageType == "kinetic" || profile.DamageType == "explosive";
        var fire = profile.DamageType == "thermal" || profile.DamageType == "explosive";

        var record = new CombatEventRecord(
            sourceId,
            targetId,
            zoneId,
            profile.DamageType,
            profile.BaseDamage,
            breach,
            fire);

        _events.Add(record);
        return record;
    }
}

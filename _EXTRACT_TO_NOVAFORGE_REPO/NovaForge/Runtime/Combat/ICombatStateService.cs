using System.Collections.Generic;

namespace Runtime.NovaForge.Combat;

public interface ICombatStateService
{
    IReadOnlyList<CombatEventRecord> GetRecentEvents();
    CombatEventRecord ApplyDamage(string sourceId, string targetId, string zoneId, DamageProfile profile);
}

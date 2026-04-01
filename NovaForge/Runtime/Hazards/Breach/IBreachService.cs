using System.Collections.Generic;

namespace Runtime.NovaForge.Hazards.Breach;

public interface IBreachService
{
    IReadOnlyList<BreachState> GetActiveBreaches();
    BreachState CreateBreach(string targetId, string zoneId, float severity);
    BreachState SealBreach(string targetId, string zoneId);
}

using System.Collections.Generic;
using System.Linq;

namespace Runtime.NovaForge.Hazards.Breach;

public sealed class BreachService : IBreachService
{
    private readonly List<BreachState> _breaches = new();

    public IReadOnlyList<BreachState> GetActiveBreaches() => _breaches;

    public BreachState CreateBreach(string targetId, string zoneId, float severity)
    {
        var state = new BreachState(targetId, zoneId, true, severity, false);
        _breaches.Add(state);
        return state;
    }

    public BreachState SealBreach(string targetId, string zoneId)
    {
        var match = _breaches.LastOrDefault(x => x.TargetId == targetId && x.ZoneId == zoneId);
        var sealedState = new BreachState(targetId, zoneId, false, match?.Severity ?? 0f, true);
        _breaches.Add(sealedState);
        return sealedState;
    }
}

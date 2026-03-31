using System.Collections.Generic;
using System.Linq;

namespace Runtime.NovaForge.Hazards.Fire;

public sealed class FireService : IFireService
{
    private readonly List<FireState> _fires = new();

    public IReadOnlyList<FireState> GetActiveFires() => _fires;

    public FireState Ignite(string targetId, string zoneId, float intensity, float heat)
    {
        var state = new FireState(targetId, zoneId, true, intensity, heat);
        _fires.Add(state);
        return state;
    }

    public FireState Extinguish(string targetId, string zoneId)
    {
        var prior = _fires.LastOrDefault(x => x.TargetId == targetId && x.ZoneId == zoneId);
        var state = new FireState(targetId, zoneId, false, 0f, prior?.Heat ?? 0f);
        _fires.Add(state);
        return state;
    }
}

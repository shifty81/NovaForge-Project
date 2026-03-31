using System.Collections.Generic;

namespace Runtime.NovaForge.Hazards.Fire;

public interface IFireService
{
    IReadOnlyList<FireState> GetActiveFires();
    FireState Ignite(string targetId, string zoneId, float intensity, float heat);
    FireState Extinguish(string targetId, string zoneId);
}

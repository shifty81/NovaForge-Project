using Runtime.NovaForge.Hazards.Breach;
using Runtime.NovaForge.Hazards.Fire;

namespace Runtime.NovaForge.Repair;

public sealed class RepairActionService : IRepairActionService
{
    private readonly IBreachService _breachService;
    private readonly IFireService _fireService;

    public RepairActionService(IBreachService breachService, IFireService fireService)
    {
        _breachService = breachService;
        _fireService = fireService;
    }

    public void ApplyBreachPatch(string targetId, string zoneId, RepairActionDef action)
    {
        // action.TimeSec / action.ToolRequired reserved for future validation.
        _breachService.SealBreach(targetId, zoneId);
    }

    public void ApplyFireSuppression(string targetId, string zoneId, RepairActionDef action)
    {
        // action.TimeSec / action.ToolRequired reserved for future validation.
        _fireService.Extinguish(targetId, zoneId);
    }

    public void ApplyHullRestore(string targetId, string zoneId, RepairActionDef action)
    {
        // action.TimeSec / action.ToolRequired reserved for future validation.
        // Stub: connect to durability / integrity services.
    }
}

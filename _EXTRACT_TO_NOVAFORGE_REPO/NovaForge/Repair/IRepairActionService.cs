namespace Runtime.NovaForge.Repair;

public interface IRepairActionService
{
    void ApplyBreachPatch(string targetId, string zoneId, RepairActionDef action);
    void ApplyFireSuppression(string targetId, string zoneId, RepairActionDef action);
    void ApplyHullRestore(string targetId, string zoneId, RepairActionDef action);
}

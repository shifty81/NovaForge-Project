using System.Collections.Generic;
using Runtime.NovaForge.Builder.Salvage;

namespace Runtime.NovaForge.Salvage;

public sealed class SalvageRuntimeService : ISalvageRuntimeService
{
    private readonly Dictionary<string, SalvageTargetRecord> _targets = new();

    public SalvageTargetRecord MarkForSalvage(string constructId, string placementId, string partId)
    {
        var target = new SalvageTargetRecord
        {
            ConstructId = constructId,
            PlacementId = placementId,
            PartId = partId,
            State = SalvageMarkState.Marked
        };

        _targets[$"{constructId}:{placementId}"] = target;
        return target;
    }

    public SalvageRecoveryRecord CutDetachRecover(SalvageTargetRecord target, string recoveryTableId)
    {
        target.State = SalvageMarkState.CutComplete;

        var recovery = new SalvageRecoveryRecord
        {
            ConstructId = target.ConstructId,
            PlacementId = target.PlacementId,
            RecoveryTableId = recoveryTableId,
            OutputItemIds = new List<string>
            {
                "hull_plate_mk1_a_recovered",
                "scrap_metal"
            }
        };

        target.State = SalvageMarkState.Recovered;
        return recovery;
    }
}

using Runtime.NovaForge.Builder.Salvage;

namespace Runtime.NovaForge.Salvage;

public interface ISalvageRuntimeService
{
    SalvageTargetRecord MarkForSalvage(string constructId, string placementId, string partId);
    SalvageRecoveryRecord CutDetachRecover(SalvageTargetRecord target, string recoveryTableId);
}

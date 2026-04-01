using System.Collections.Generic;
using Runtime.NovaForge.Builder;
using Runtime.NovaForge.Builder.Placement;
using Runtime.NovaForge.Builder.Validation;
using Runtime.NovaForge.Salvage;

namespace Runtime.NovaForge.DevWorld.Services;

public sealed class BuilderSalvageSmokeTestService
{
    private readonly IBuilderPlacementService _builderPlacementService;
    private readonly IConstructValidationService _constructValidationService;
    private readonly ISalvageRuntimeService _salvageRuntimeService;

    public BuilderSalvageSmokeTestService(
        IBuilderPlacementService builderPlacementService,
        IConstructValidationService constructValidationService,
        ISalvageRuntimeService salvageRuntimeService)
    {
        _builderPlacementService = builderPlacementService;
        _constructValidationService = constructValidationService;
        _salvageRuntimeService = salvageRuntimeService;
    }

    public IReadOnlyList<string> Run(string constructId)
    {
        var log = new List<string>();

        var preview = _builderPlacementService.PreviewPlacement(constructId, "hull_plate_mk1_a", "grid_1m", 2, 0, 0, 0);
        log.Add($"Preview valid: {preview.IsValid}");

        var committed = _builderPlacementService.CommitPlacement(preview);
        log.Add($"Committed state: {committed.State}");

        var validation = _constructValidationService.ValidateConstruct(constructId);
        log.Add($"Validation count: {validation.Count}");

        var welded = _builderPlacementService.WeldPlacement(constructId, committed.PlacementId);
        log.Add($"Welded state: {welded.State}");

        var damaged = _builderPlacementService.ApplyDamage(constructId, committed.PlacementId, 75.0f);
        log.Add($"Damaged durability: {damaged.Durability}");

        var target = _salvageRuntimeService.MarkForSalvage(constructId, committed.PlacementId, committed.PartId);
        log.Add($"Salvage mark state: {target.State}");

        var recovery = _salvageRuntimeService.CutDetachRecover(target, "recovery_hull_plate_mk1_a");
        log.Add($"Recovery outputs: {string.Join(",", recovery.OutputItemIds)}");

        return log;
    }
}

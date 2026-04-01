using System;
using System.Collections.Generic;
using Runtime.NovaForge.Builder.Placement;

namespace Runtime.NovaForge.Builder;

public sealed class BuilderPlacementService : IBuilderPlacementService
{
    private readonly Dictionary<string, BuilderPlacementRecord> _placements = new();

    public PlacementPreviewResult PreviewPlacement(string constructId, string partId, string snapProfileId, int x, int y, int z, int rotation)
    {
        var placementId = $"preview_{Guid.NewGuid():N}";
        var record = new BuilderPlacementRecord
        {
            PlacementId = placementId,
            ConstructId = constructId,
            PartId = partId,
            SnapProfileId = snapProfileId,
            GridX = x,
            GridY = y,
            GridZ = z,
            Rotation = rotation,
            Durability = 250.0f,
            State = PlacementState.Ghost
        };

        return new PlacementPreviewResult
        {
            IsValid = true,
            Reason = "Preview accepted by scaffold service.",
            PreviewRecord = record
        };
    }

    public BuilderPlacementRecord CommitPlacement(PlacementPreviewResult preview)
    {
        if (!preview.IsValid || preview.PreviewRecord is null)
        {
            throw new InvalidOperationException("Cannot commit invalid placement preview.");
        }

        var record = preview.PreviewRecord;
        record.State = PlacementState.PlacedUnwelded;
        _placements[record.PlacementId] = record;
        return record;
    }

    public BuilderPlacementRecord WeldPlacement(string constructId, string placementId)
    {
        var record = GetPlacement(constructId, placementId);
        record.State = PlacementState.Welded;
        return record;
    }

    public BuilderPlacementRecord ApplyDamage(string constructId, string placementId, float damageAmount)
    {
        var record = GetPlacement(constructId, placementId);
        record.Durability = Math.Max(0.0f, record.Durability - damageAmount);
        record.State = PlacementState.Damaged;
        return record;
    }

    private BuilderPlacementRecord GetPlacement(string constructId, string placementId)
    {
        if (!_placements.TryGetValue(placementId, out var record) || record.ConstructId != constructId)
        {
            throw new KeyNotFoundException($"Placement not found: {constructId}/{placementId}");
        }

        return record;
    }
}

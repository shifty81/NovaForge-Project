using Runtime.NovaForge.Builder.Placement;

namespace Runtime.NovaForge.Builder;

public interface IBuilderPlacementService
{
    PlacementPreviewResult PreviewPlacement(string constructId, string partId, string snapProfileId, int x, int y, int z, int rotation);
    BuilderPlacementRecord CommitPlacement(PlacementPreviewResult preview);
    BuilderPlacementRecord WeldPlacement(string constructId, string placementId);
    BuilderPlacementRecord ApplyDamage(string constructId, string placementId, float damageAmount);
}

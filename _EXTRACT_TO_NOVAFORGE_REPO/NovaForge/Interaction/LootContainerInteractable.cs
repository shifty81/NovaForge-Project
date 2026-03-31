namespace Runtime.NovaForge.Interaction;

public sealed class LootContainerInteractable : IInteractable
{
    public string Id { get; init; } = "loot_container_a";
    public string DisplayName { get; init; } = "Salvage Crate";
    public string InteractionVerb => "Open";

    public bool CanInteract(string actorId) => true;

    public void Interact(string actorId)
    {
        // TODO: open container UI and route through server-authoritative inventory transaction flow.
    }
}

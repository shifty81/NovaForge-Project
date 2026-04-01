namespace Runtime.NovaForge.Interaction;

public interface IInteractable
{
    string Id { get; }
    string DisplayName { get; }
    string InteractionVerb { get; }
    bool CanInteract(string actorId);
    void Interact(string actorId);
}

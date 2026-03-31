using System.Collections.Generic;

namespace Runtime.NovaForge.Interaction;

public interface IInteractionService
{
    void Register(IInteractable interactable);
    IInteractable? Resolve(string id);
}

public sealed class InteractionService : IInteractionService
{
    private readonly Dictionary<string, IInteractable> _interactables = new();

    public void Register(IInteractable interactable)
    {
        _interactables[interactable.Id] = interactable;
    }

    public IInteractable? Resolve(string id)
    {
        _interactables.TryGetValue(id, out var interactable);
        return interactable;
    }
}

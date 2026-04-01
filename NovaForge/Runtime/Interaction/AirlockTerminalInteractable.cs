namespace Runtime.NovaForge.Interaction;

public sealed class AirlockTerminalInteractable : IInteractable
{
    public string Id { get; init; } = "airlock_terminal_a";
    public string DisplayName { get; init; } = "Airlock Control Terminal";
    public string InteractionVerb => "Operate";
    public bool IsPowered { get; set; } = true;

    public bool CanInteract(string actorId) => IsPowered;

    public void Interact(string actorId)
    {
        // TODO: bind to real airlock controller and pressure-room service.
    }
}

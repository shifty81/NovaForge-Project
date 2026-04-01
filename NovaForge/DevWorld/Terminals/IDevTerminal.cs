namespace Runtime.NovaForge.DevWorld.Terminals
{
    public interface IDevTerminal
    {
        string TerminalId { get; }
        string DisplayName { get; }
        void Interact();
    }
}

using Runtime.NovaForge.Player;

namespace Runtime.NovaForge.SaveLoad;

public sealed class RigSaveState
{
    public required string SaveId { get; init; }
    public required PlayerRigState RigState { get; init; }
}

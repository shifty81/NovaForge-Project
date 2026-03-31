using System.Collections.Generic;

namespace Runtime.NovaForge.Player;

public sealed class PlayerRigState
{
    public string PlayerId { get; set; } = string.Empty;
    public float Health { get; set; } = 100f;
    public float Integrity { get; set; } = 100f;
    public float Oxygen { get; set; } = 100f;
    public float Power { get; set; } = 100f;
    public float Heat { get; set; } = 0f;
    public string ActiveMode { get; set; } = "OnFoot";
    public Dictionary<string, string> EquippedSlots { get; } = new();
    public List<string> QuickSlots { get; } = new();
}

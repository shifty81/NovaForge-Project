using System.Threading;
using System.Threading.Tasks;

namespace Runtime.NovaForge.Player;

public interface IRigBootstrapService
{
    Task<PlayerRigState> SpawnDefaultRigAsync(string playerId, CancellationToken cancellationToken = default);
}

public sealed class RigBootstrapService : IRigBootstrapService
{
    public Task<PlayerRigState> SpawnDefaultRigAsync(string playerId, CancellationToken cancellationToken = default)
    {
        var state = new PlayerRigState
        {
            PlayerId = playerId,
            Health = 100f,
            Integrity = 100f,
            Oxygen = 100f,
            Power = 100f,
            Heat = 0f,
            ActiveMode = "OnFoot"
        };

        state.EquippedSlots["Helmet"] = "rig_helmet_mk1";
        state.EquippedSlots["Torso"] = "rig_torso_mk1";
        state.EquippedSlots["Legs"] = "rig_legs_mk1";
        state.EquippedSlots["Boots"] = "rig_boots_mk1";
        state.EquippedSlots["Backpack"] = "rig_pack_mk1";
        state.EquippedSlots["PrimaryTool"] = "cutter_tool_mk1";
        state.EquippedSlots["SecondaryTool"] = "scanner_tool_mk1";

        state.QuickSlots.Add("cutter_tool_mk1");
        state.QuickSlots.Add("sealant_kit_mk1");
        state.QuickSlots.Add("scanner_tool_mk1");
        state.QuickSlots.Add("oxygen_canister_mk1");

        return Task.FromResult(state);
    }
}

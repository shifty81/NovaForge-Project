using Runtime.NovaForge.Player;

namespace Runtime.NovaForge.DevWorld.Terminals
{
    public sealed class RigLoadoutTerminal : IDevTerminal
    {
        private readonly ILoadoutService _loadoutService;

        public RigLoadoutTerminal(ILoadoutService loadoutService)
        {
            _loadoutService = loadoutService;
        }

        public string TerminalId => "terminal_rig_loadout";
        public string DisplayName => "Rig Loadout Terminal";

        public void Interact()
        {
            _loadoutService.ApplyLoadout("starter_dev_loadout");
        }
    }
}

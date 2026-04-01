using Runtime.NovaForge.Factions;

namespace Runtime.NovaForge.DevWorld.Terminals
{
    public sealed class FactionDebugTerminal : IDevTerminal
    {
        private readonly IFactionDebugService _factionDebugService;

        public FactionDebugTerminal(IFactionDebugService factionDebugService)
        {
            _factionDebugService = factionDebugService;
        }

        public string TerminalId => "faction_debug_a";
        public string DisplayName => "Faction Debug Terminal";

        public void Interact()
        {
            _factionDebugService.OpenStandingView("industrial_union");
        }
    }
}

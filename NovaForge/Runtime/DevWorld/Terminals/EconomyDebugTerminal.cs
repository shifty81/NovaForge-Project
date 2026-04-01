using Runtime.NovaForge.Economy;

namespace Runtime.NovaForge.DevWorld.Terminals
{
    public sealed class EconomyDebugTerminal : IDevTerminal
    {
        private readonly IEconomyDebugService _economyDebugService;

        public EconomyDebugTerminal(IEconomyDebugService economyDebugService)
        {
            _economyDebugService = economyDebugService;
        }

        public string TerminalId => "market_debug_a";
        public string DisplayName => "Economy Debug Terminal";

        public void Interact()
        {
            _economyDebugService.OpenSnapshot("station_orbit_12_market");
        }
    }
}

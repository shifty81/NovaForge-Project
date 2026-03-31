using Runtime.NovaForge.Missions;

namespace Runtime.NovaForge.DevWorld.Terminals
{
    public sealed class MissionBoardTerminal : IDevTerminal
    {
        private readonly IMissionService _missionService;

        public MissionBoardTerminal(IMissionService missionService)
        {
            _missionService = missionService;
        }

        public string TerminalId => "mission_board_a";
        public string DisplayName => "Mission Board";

        public void Interact()
        {
            _missionService.Offer("mission_dev_salvage_intro");
        }
    }
}

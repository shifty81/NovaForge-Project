using Runtime.NovaForge.Missions;
using Runtime.NovaForge.SaveLoad;

namespace Runtime.NovaForge.DevWorld.Services
{
    public sealed class DevWorldSmokeTestService
    {
        private readonly IMissionService _missionService;
        private readonly ISaveLoadService _saveLoadService;

        public DevWorldSmokeTestService(IMissionService missionService, ISaveLoadService saveLoadService)
        {
            _missionService = missionService;
            _saveLoadService = saveLoadService;
        }

        public void RunQuickValidation()
        {
            _missionService.Offer("mission_dev_salvage_intro");
            _saveLoadService.Save("dev_smoke");
            _saveLoadService.Load("dev_smoke");
        }
    }
}

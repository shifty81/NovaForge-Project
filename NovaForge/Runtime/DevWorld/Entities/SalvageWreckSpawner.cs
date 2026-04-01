using Runtime.NovaForge.Salvage;

namespace Runtime.NovaForge.DevWorld.Entities
{
    public sealed class SalvageWreckSpawner
    {
        private readonly ISalvageTestService _salvageTestService;

        public SalvageWreckSpawner(ISalvageTestService salvageTestService)
        {
            _salvageTestService = salvageTestService;
        }

        public void SpawnWreck()
        {
            _salvageTestService.SpawnTestWreck("salvage_lane_spawn_a");
        }
    }
}

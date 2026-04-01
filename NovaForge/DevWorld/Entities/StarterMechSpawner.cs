using Runtime.NovaForge.Constructs;

namespace Runtime.NovaForge.DevWorld.Entities
{
    public sealed class StarterMechSpawner
    {
        private readonly IConstructSpawnService _constructSpawnService;

        public StarterMechSpawner(IConstructSpawnService constructSpawnService)
        {
            _constructSpawnService = constructSpawnService;
        }

        public void SpawnDefaultMech()
        {
            _constructSpawnService.Spawn("mech_utility_dev", "mech_bay_spawn");
        }
    }
}

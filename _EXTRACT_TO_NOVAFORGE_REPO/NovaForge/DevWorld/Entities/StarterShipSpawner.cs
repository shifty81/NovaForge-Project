using Runtime.NovaForge.Constructs;

namespace Runtime.NovaForge.DevWorld.Entities
{
    public sealed class StarterShipSpawner
    {
        private readonly IConstructSpawnService _constructSpawnService;

        public StarterShipSpawner(IConstructSpawnService constructSpawnService)
        {
            _constructSpawnService = constructSpawnService;
        }

        public void SpawnDefaultShip()
        {
            _constructSpawnService.Spawn("starter_ship_dev", "ship_hangar_spawn");
        }
    }
}

using Runtime.NovaForge.Combat;

namespace Runtime.NovaForge.DevWorld.Entities
{
    public sealed class CombatDummySpawner
    {
        private readonly ICombatTestService _combatTestService;

        public CombatDummySpawner(ICombatTestService combatTestService)
        {
            _combatTestService = combatTestService;
        }

        public void SpawnDummy()
        {
            _combatTestService.SpawnDummy("combat_range_spawn_a");
        }
    }
}

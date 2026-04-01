namespace Runtime.NovaForge.Player
{
    public interface IPlayerSpawnService
    {
        void SpawnAt(string spawnPointId);
    }

    public interface ILoadoutService
    {
        void ApplyLoadout(string loadoutId);
    }
}

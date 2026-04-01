using System;
using Runtime.NovaForge.Interaction;
using Runtime.NovaForge.Missions;
using Runtime.NovaForge.Player;

namespace Runtime.NovaForge.DevWorld
{
    /// <summary>
    /// Deterministic bootstrap for the NovaForge Dev World used by Atlas Suite playtesting.
    /// </summary>
    public sealed class DevWorldBootstrap
    {
        private readonly ISceneLoader _sceneLoader;
        private readonly IPlayerSpawnService _playerSpawnService;
        private readonly IMissionService _missionService;
        private readonly IDevWorldRegistry _registry;

        public const string ScenePath = "Projects/NovaForge/Scenes/dev_world.scene.json";

        public DevWorldBootstrap(
            ISceneLoader sceneLoader,
            IPlayerSpawnService playerSpawnService,
            IMissionService missionService,
            IDevWorldRegistry registry)
        {
            _sceneLoader = sceneLoader;
            _playerSpawnService = playerSpawnService;
            _missionService = missionService;
            _registry = registry;
        }

        public void Start()
        {
            _sceneLoader.Load(ScenePath);
            _playerSpawnService.SpawnAt("spawn_dev_room");
            _missionService.Prime("mission_dev_salvage_intro");
            _registry.RegisterDefaults();
        }

        public void ResetWorld()
        {
            _registry.ResetAll();
            Start();
        }
    }

    public interface ISceneLoader
    {
        void Load(string scenePath);
    }

    public interface IDevWorldRegistry
    {
        void RegisterDefaults();
        void ResetAll();
    }
}

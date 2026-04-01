using System.Collections.Generic;

namespace Runtime.NovaForge.DevWorld
{
    public sealed class DevWorldSceneDefinition
    {
        public string Id { get; init; } = "dev_world";
        public string SpawnPointId { get; init; } = "spawn_dev_room";
        public int DeterministicSeed { get; init; } = 424242;
        public IReadOnlyList<string> ZoneIds { get; init; } = new[]
        {
            "spawn_room",
            "airlock_room",
            "eva_lane",
            "mech_bay",
            "ship_hangar",
            "builder_pad",
            "combat_range",
            "salvage_lane"
        };
    }
}

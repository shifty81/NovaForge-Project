using System.Collections.Generic;

namespace AtlasSuite.Modules.Project;

/// <summary>Deserialized representation of a NovaForge level file.</summary>
public sealed class NovaForgeLevel
{
    public string LevelName { get; init; } = string.Empty;
    public string LevelPath { get; init; } = string.Empty;
    public List<LevelEntity> Entities { get; init; } = [];
}

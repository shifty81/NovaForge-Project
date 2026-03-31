using System.Collections.Generic;

namespace AtlasSuite.Modules.Project;

/// <summary>Lightweight representation of an entity inside a loaded NovaForge level.</summary>
public sealed class LevelEntity
{
    public string Id { get; init; } = string.Empty;
    public string Name { get; init; } = string.Empty;
    public string Type { get; init; } = string.Empty;
    public Dictionary<string, object> Properties { get; init; } = [];
}

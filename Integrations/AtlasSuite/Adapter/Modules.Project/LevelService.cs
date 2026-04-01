using System.Text.Json;
using System.Text.Json.Serialization;

namespace AtlasSuite.Modules.Project;

/// <summary>
/// Loads and saves NovaForge JSON level files.
/// Also provides a file-system watcher: when the active level file is modified
/// externally, <see cref="LevelFileChanged"/> is raised so the UI can offer a
/// reload prompt.
/// </summary>
public sealed class LevelService : IDisposable
{
    private FileSystemWatcher? _watcher;
    private bool _disposed;

    public event EventHandler<LevelLoadedEventArgs>? LevelLoaded;
    public event EventHandler<LevelLoadFailedEventArgs>? LevelLoadFailed;

    /// <summary>
    /// Raised when the currently-watched level file is modified externally.
    /// The string argument is the full path of the changed file.
    /// </summary>
    public event EventHandler<string>? LevelFileChanged;

    public NovaForgeLevel? CurrentLevel { get; private set; }

    /// <summary>
    /// Deserialises the JSON level file at <paramref name="path"/> and fires
    /// <see cref="LevelLoaded"/> with the resulting <see cref="NovaForgeLevel"/>.
    /// Also (re-)starts the file-watcher on the parent directory.
    /// </summary>
    public async Task OpenLevelAsync(string path, CancellationToken cancellationToken = default)
    {
        ArgumentException.ThrowIfNullOrWhiteSpace(path);

        try
        {
            await using var stream = File.OpenRead(path);
            var dto = await JsonSerializer.DeserializeAsync<LevelFileDto>(
                stream,
                LevelSerializerOptions.Default,
                cancellationToken).ConfigureAwait(false);

            var level = MapToLevel(path, dto);
            CurrentLevel = level;
            StartWatching(path);
            LevelLoaded?.Invoke(this, new LevelLoadedEventArgs(level));
        }
        catch (Exception ex)
        {
            LevelLoadFailed?.Invoke(this, new LevelLoadFailedEventArgs(path, ex.Message));
        }
    }

    /// <summary>
    /// Persists the current in-memory level (with any inspector edits applied)
    /// back to the JSON file at <see cref="NovaForgeLevel.LevelPath"/>.
    /// </summary>
    public async Task SaveLevelAsync(CancellationToken cancellationToken = default)
    {
        if (CurrentLevel is null)
            return;

        await SaveLevelAsync(CurrentLevel, cancellationToken).ConfigureAwait(false);
    }

    /// <summary>
    /// Persists <paramref name="level"/> to <see cref="NovaForgeLevel.LevelPath"/>.
    /// The file-watcher is temporarily suppressed so the save does not trigger a
    /// <see cref="LevelFileChanged"/> notification on the same process.
    /// </summary>
    public async Task SaveLevelAsync(NovaForgeLevel level, CancellationToken cancellationToken = default)
    {
        ArgumentNullException.ThrowIfNull(level);

        // Pause the watcher so we don't react to our own save
        if (_watcher is not null)
            _watcher.EnableRaisingEvents = false;

        try
        {
            var dto = MapToDto(level);
            var json = JsonSerializer.Serialize(dto, LevelSerializerOptions.Write);

            // Ensure the directory exists
            var dir = Path.GetDirectoryName(level.LevelPath);
            if (!string.IsNullOrWhiteSpace(dir))
                Directory.CreateDirectory(dir);

            await File.WriteAllTextAsync(level.LevelPath, json, cancellationToken).ConfigureAwait(false);

            // Update cached level reference
            CurrentLevel = level;
        }
        finally
        {
            if (_watcher is not null)
                _watcher.EnableRaisingEvents = true;
        }
    }

    /// <summary>
    /// Starts watching the directory that contains <paramref name="levelPath"/>
    /// for changes to the specific file.
    /// </summary>
    public void StartWatching(string levelPath)
    {
        StopWatching();

        var dir = Path.GetDirectoryName(levelPath);
        var file = Path.GetFileName(levelPath);
        if (string.IsNullOrWhiteSpace(dir) || string.IsNullOrWhiteSpace(file))
            return;

        _watcher = new FileSystemWatcher(dir, file)
        {
            NotifyFilter = NotifyFilters.LastWrite | NotifyFilters.Size,
            EnableRaisingEvents = true
        };

        _watcher.Changed += OnWatcherChanged;
    }

    /// <summary>Stops the file-system watcher if one is active.</summary>
    public void StopWatching()
    {
        if (_watcher is null)
            return;

        _watcher.EnableRaisingEvents = false;
        _watcher.Changed -= OnWatcherChanged;
        _watcher.Dispose();
        _watcher = null;
    }

    public void Dispose()
    {
        if (_disposed)
            return;
        _disposed = true;
        StopWatching();
    }

    // ── Private helpers ───────────────────────────────────────────────────────

    private void OnWatcherChanged(object sender, FileSystemEventArgs e)
    {
        LevelFileChanged?.Invoke(this, e.FullPath);
    }

    private static NovaForgeLevel MapToLevel(string path, LevelFileDto? dto)
    {
        if (dto is null)
        {
            return new NovaForgeLevel
            {
                LevelName = Path.GetFileNameWithoutExtension(path),
                LevelPath = path
            };
        }

        var entities = (dto.Entities ?? [])
            .Select(e => new LevelEntity
            {
                Id = e.Id ?? Guid.NewGuid().ToString(),
                Name = e.Name ?? "(unnamed)",
                Type = e.Type ?? "Unknown",
                Properties = e.Properties ?? []
            })
            .ToList();

        return new NovaForgeLevel
        {
            LevelName = dto.LevelName ?? Path.GetFileNameWithoutExtension(path),
            LevelPath = path,
            Entities = entities
        };
    }

    private static LevelFileDto MapToDto(NovaForgeLevel level)
    {
        return new LevelFileDto
        {
            LevelName = level.LevelName,
            Entities = level.Entities
                .Select(e => new EntityDto
                {
                    Id = e.Id,
                    Name = e.Name,
                    Type = e.Type,
                    Properties = e.Properties.ToDictionary(kv => kv.Key, kv => (object?)kv.Value)
                })
                .ToList()
        };
    }

    // ── Private DTOs ──────────────────────────────────────────────────────────

    private sealed class LevelFileDto
    {
        [JsonPropertyName("levelName")]
        public string? LevelName { get; init; }

        [JsonPropertyName("entities")]
        public List<EntityDto>? Entities { get; init; }
    }

    private sealed class EntityDto
    {
        [JsonPropertyName("id")]
        public string? Id { get; init; }

        [JsonPropertyName("name")]
        public string? Name { get; init; }

        [JsonPropertyName("type")]
        public string? Type { get; init; }

        [JsonPropertyName("properties")]
        public Dictionary<string, object?>? Properties { get; init; }
    }

    private static class LevelSerializerOptions
    {
        internal static readonly JsonSerializerOptions Default = new()
        {
            PropertyNameCaseInsensitive = true
        };

        internal static readonly JsonSerializerOptions Write = new()
        {
            WriteIndented = true,
            DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull
        };
    }
}

/// <summary>Event arguments raised when a level is successfully loaded.</summary>
public sealed class LevelLoadedEventArgs(NovaForgeLevel level) : EventArgs
{
    public NovaForgeLevel Level { get; } = level;
}

/// <summary>Event arguments raised when level loading fails.</summary>
public sealed class LevelLoadFailedEventArgs(string path, string reason) : EventArgs
{
    public string Path { get; } = path;
    public string Reason { get; } = reason;
}


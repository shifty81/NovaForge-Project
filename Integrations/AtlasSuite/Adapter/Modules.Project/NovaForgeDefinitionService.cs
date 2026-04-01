using System.Text.Json;
using System.Text.Json.Nodes;
using AtlasSuite.Core.Abstractions;

namespace AtlasSuite.Modules.Project;

/// <summary>
/// Discovers and loads NovaForge data-definition JSON files from the
/// <c>NovaForge/Data/</c> tree.  Each JSON file is a dictionary where the
/// top-level keys are record IDs and the values are the record objects.
///
/// Saves are done in-place: the service reads the file, replaces (or adds)
/// the affected record, and writes it back as indented JSON.
///
/// Implements <see cref="IDefinitionService"/> so the Suite's
/// <c>DataEditorViewModel</c> can accept any project's definition service
/// without coupling to this concrete type.
/// </summary>
public sealed class NovaForgeDefinitionService : IDefinitionService
{
    private readonly string _dataRoot;

    /// <param name="dataRoot">Absolute path to the <c>NovaForge/Data/</c> folder.</param>
    public NovaForgeDefinitionService(string dataRoot)
    {
        _dataRoot = dataRoot;
    }

    // ── Category discovery ────────────────────────────────────────────────────

    /// <summary>Returns the top-level category sub-folders present under the data root.</summary>
    public IReadOnlyList<string> GetCategories()
    {
        if (!Directory.Exists(_dataRoot))
            return [];

        return Directory.EnumerateDirectories(_dataRoot)
            .Select(Path.GetFileName)
            .OfType<string>()
            .OrderBy(s => s)
            .ToList();
    }

    /// <summary>
    /// Returns the definition files available inside <paramref name="category"/>.
    /// Each entry is the file name without extension, e.g. "frigates".
    /// </summary>
    public IReadOnlyList<string> GetDefinitionFiles(string category)
    {
        var dir = Path.Combine(_dataRoot, category);
        if (!Directory.Exists(dir))
            return [];

        return Directory.EnumerateFiles(dir, "*.json")
            .Select(f => Path.GetFileNameWithoutExtension(f))
            .OfType<string>()
            .OrderBy(s => s)
            .ToList();
    }

    // ── Record load ───────────────────────────────────────────────────────────

    /// <summary>
    /// Loads all records from <paramref name="category"/>/<paramref name="file"/>.json
    /// and returns them as a list of flat <see cref="DefinitionRecord"/> objects.
    /// Returns an empty list if the file does not exist or cannot be parsed.
    /// </summary>
    public async Task<IReadOnlyList<DefinitionRecord>> LoadRecordsAsync(
        string category,
        string file,
        CancellationToken cancellationToken = default)
    {
        var path = Path.Combine(_dataRoot, category, $"{file}.json");
        if (!File.Exists(path))
            return [];

        try
        {
            var json = await File.ReadAllTextAsync(path, cancellationToken).ConfigureAwait(false);
            var root = JsonNode.Parse(json);
            if (root is not JsonObject dict)
                return [];

            var records = new List<DefinitionRecord>();
            foreach (var kv in dict)
            {
                var fields = new Dictionary<string, string>();
                if (kv.Value is JsonObject obj)
                {
                    foreach (var field in obj)
                    {
                        fields[field.Key] = field.Value?.ToString() ?? string.Empty;
                    }
                }

                records.Add(new DefinitionRecord(
                    Id: kv.Key,
                    Category: category,
                    File: file,
                    FilePath: path,
                    Fields: fields));
            }

            return records;
        }
        catch
        {
            return [];
        }
    }

    // ── Record save ───────────────────────────────────────────────────────────

    /// <summary>
    /// Writes a single <see cref="DefinitionRecord"/> back to its source file.
    /// The file-level dictionary is preserved; only the record keyed by
    /// <see cref="DefinitionRecord.Id"/> is replaced.
    /// </summary>
    public async Task SaveRecordAsync(
        DefinitionRecord record,
        CancellationToken cancellationToken = default)
    {
        ArgumentNullException.ThrowIfNull(record);

        var dir = Path.GetDirectoryName(record.FilePath);
        if (!string.IsNullOrWhiteSpace(dir))
            Directory.CreateDirectory(dir);

        // Read existing file (or start fresh)
        JsonObject root;
        if (File.Exists(record.FilePath))
        {
            try
            {
                var existing = await File.ReadAllTextAsync(record.FilePath, cancellationToken)
                    .ConfigureAwait(false);
                root = JsonNode.Parse(existing) as JsonObject ?? [];
            }
            catch
            {
                root = [];
            }
        }
        else
        {
            root = [];
        }

        // Rebuild the record object from the flat Fields dictionary
        var recordObj = new JsonObject();
        foreach (var kv in record.Fields)
        {
            // Attempt to preserve numeric types; fall back to string
            if (long.TryParse(kv.Value, out var longVal))
                recordObj[kv.Key] = JsonValue.Create(longVal);
            else if (double.TryParse(kv.Value,
                System.Globalization.NumberStyles.Any,
                System.Globalization.CultureInfo.InvariantCulture,
                out var dblVal))
                recordObj[kv.Key] = JsonValue.Create(dblVal);
            else
                recordObj[kv.Key] = JsonValue.Create(kv.Value);
        }

        root[record.Id] = recordObj;

        var json = root.ToJsonString(new JsonSerializerOptions { WriteIndented = true });
        await File.WriteAllTextAsync(record.FilePath, json, cancellationToken)
            .ConfigureAwait(false);
    }
}

/// <summary>
/// An immutable snapshot of a single definition record loaded from a JSON file.
/// </summary>
/// <param name="Id">Primary key of the record (top-level JSON key).</param>
/// <param name="Category">Category folder name (e.g. "ships", "Factions").</param>
/// <param name="File">Base file name without extension (e.g. "frigates").</param>
/// <param name="FilePath">Absolute path to the source JSON file.</param>
/// <param name="Fields">Flat field dictionary — all values serialized to strings.</param>
public sealed record DefinitionRecord(
    string Id,
    string Category,
    string File,
    string FilePath,
    Dictionary<string, string> Fields);

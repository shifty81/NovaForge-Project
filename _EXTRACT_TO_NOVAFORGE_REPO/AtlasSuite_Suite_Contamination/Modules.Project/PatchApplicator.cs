using System.Text.Json;
using System.Text.Json.Nodes;

namespace AtlasSuite.Modules.Project;

/// <summary>
/// Applies an approved AI content-proposal diff to the appropriate
/// <c>NovaForge/Data/</c> JSON file (M9 patch-write path).
/// </summary>
/// <remarks>
/// <para>
/// A proposal's <c>DiffPreview</c> string uses a simple unified-diff-like
/// format where added lines are prefixed with <c>+</c>.  The applicator strips
/// those prefixes, parses the resulting JSON object, and appends it to the JSON
/// array stored in the proposal's <c>TargetFile</c>.
/// </para>
/// <para>
/// If the target file does not yet exist it is created with a single-element
/// array.  If the file already contains an entry with the same <c>id</c> field
/// the operation throws <see cref="PatchApplicatorException"/> to prevent
/// duplicates.
/// </para>
/// </remarks>
public sealed class PatchApplicator
{
    private readonly string _baseDir;

    /// <param name="baseDir">
    /// Absolute path to the repository root (or any directory whose sub-paths
    /// match the <c>TargetFile</c> values stored in proposals).
    /// </param>
    public PatchApplicator(string baseDir)
    {
        _baseDir = baseDir ?? throw new ArgumentNullException(nameof(baseDir));
    }

    // ── Public API ────────────────────────────────────────────────────

    /// <summary>
    /// Parse the diff, append the new entry to the target file, and return
    /// the number of bytes written.
    /// </summary>
    /// <param name="targetFile">
    /// Repo-relative path (e.g. <c>NovaForge/Data/Ships/ships.json</c>).
    /// </param>
    /// <param name="diffPreview">
    /// Unified-diff fragment produced by
    /// <c>ContentAuthorService.propose_*</c> — each added line prefixed
    /// with <c>+</c>.
    /// </param>
    /// <returns>Bytes written to the target file.</returns>
    /// <exception cref="PatchApplicatorException">
    /// Thrown when the diff cannot be parsed or the target contains a
    /// duplicate <c>id</c>.
    /// </exception>
    public int Apply(string targetFile, string diffPreview)
    {
        var entry = ParseDiff(diffPreview);
        var path  = Path.Combine(_baseDir, targetFile);

        Directory.CreateDirectory(Path.GetDirectoryName(path)!);

        JsonArray array;

        if (File.Exists(path))
        {
            var text = File.ReadAllText(path);
            var node = JsonNode.Parse(text)
                ?? throw new PatchApplicatorException($"Target file {path} is empty or null.");

            if (node is not JsonArray existing)
                throw new PatchApplicatorException(
                    $"Target file {path} does not contain a JSON array (found {node.GetType().Name}).");

            // Duplicate-ID guard
            var entryId = entry["id"]?.GetValue<string>();
            if (entryId is not null)
            {
                foreach (var item in existing)
                {
                    if (item?["id"]?.GetValue<string>() == entryId)
                        throw new PatchApplicatorException(
                            $"Entry with id '{entryId}' already exists in {path}.");
                }
            }

            existing.Add(JsonNode.Parse(entry.ToJsonString())!);
            array = existing;
        }
        else
        {
            array = new JsonArray(JsonNode.Parse(entry.ToJsonString())!);
        }

        var options = new JsonSerializerOptions { WriteIndented = true };
        var output  = array.ToJsonString(options) + "\n";
        File.WriteAllText(path, output);
        return output.Length;
    }

    /// <summary>
    /// Parse the diff preview without writing to disk.
    /// </summary>
    /// <returns>The <see cref="JsonObject"/> that would be appended.</returns>
    public JsonObject Preview(string diffPreview) => ParseDiff(diffPreview);

    // ── Internals ─────────────────────────────────────────────────────

    private static JsonObject ParseDiff(string diffPreview)
    {
        var lines = new List<string>();
        foreach (var line in diffPreview.Split('\n'))
        {
            var stripped = line.TrimStart();
            if (stripped.StartsWith('+'))
            {
                var content = stripped[1..];
                lines.Add(content.StartsWith(' ') ? content[1..] : content);
            }
            else if (!stripped.StartsWith('-'))
            {
                // context line
                lines.Add(line);
            }
        }

        var raw = string.Join('\n', lines).Trim();

        try
        {
            var node = JsonNode.Parse(raw)
                ?? throw new PatchApplicatorException("diff_preview parsed to null.");

            // Handle array-wrapped single-element diffs
            if (node is JsonArray arr)
            {
                if (arr.Count != 1)
                    throw new PatchApplicatorException(
                        "diff_preview array contains != 1 element; cannot determine new entry.");
                node = arr[0]!;
            }

            return node as JsonObject
                ?? throw new PatchApplicatorException(
                    $"diff_preview root is not a JSON object (found {node.GetType().Name}).");
        }
        catch (JsonException ex)
        {
            throw new PatchApplicatorException(
                $"Could not parse diff_preview as JSON: {ex.Message}\nRaw content:\n{raw}", ex);
        }
    }
}

/// <summary>Exception raised by <see cref="PatchApplicator"/> on patch failures.</summary>
public sealed class PatchApplicatorException : Exception
{
    /// <inheritdoc cref="Exception(string)"/>
    public PatchApplicatorException(string message) : base(message) { }

    /// <inheritdoc cref="Exception(string, Exception)"/>
    public PatchApplicatorException(string message, Exception inner) : base(message, inner) { }
}

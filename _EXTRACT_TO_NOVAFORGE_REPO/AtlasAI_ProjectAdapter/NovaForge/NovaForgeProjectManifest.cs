// NovaForgeProjectManifest.cs
// AtlasAI-side loader for the NovaForge project manifest.
//
// This class reads novaforge.project.json from the Shared/ProjectManifests folder
// and exposes its contents as a strongly-typed model for the AtlasAI project adapter.

using System;
using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace AtlasAI.ProjectAdapters.NovaForge
{
    public sealed class ProjectCapabilitiesModel
    {
        [JsonPropertyName("supportsViewportAttach")]
        public bool SupportsViewportAttach { get; init; }

        [JsonPropertyName("supportsLivePatch")]
        public bool SupportsLivePatch { get; init; }

        [JsonPropertyName("supportsAISession")]
        public bool SupportsAISession { get; init; }

        [JsonPropertyName("supportsProjectIndexing")]
        public bool SupportsProjectIndexing { get; init; }

        [JsonPropertyName("supportsMultiWorkspace")]
        public bool SupportsMultiWorkspace { get; init; }
    }

    public sealed class BuildTargetModel
    {
        [JsonPropertyName("name")]
        public string Name { get; init; } = string.Empty;

        [JsonPropertyName("displayName")]
        public string DisplayName { get; init; } = string.Empty;

        [JsonPropertyName("configuration")]
        public string Configuration { get; init; } = "Debug";

        [JsonPropertyName("platform")]
        public string Platform { get; init; } = "Win64";
    }

    public sealed class BridgeConfigModel
    {
        [JsonPropertyName("transport")]
        public string Transport { get; init; } = "rest+websocket";

        [JsonPropertyName("host")]
        public string Host { get; init; } = "localhost";

        [JsonPropertyName("restPort")]
        public int RestPort { get; init; } = 57100;

        [JsonPropertyName("wsPort")]
        public int WsPort { get; init; } = 57101;

        [JsonPropertyName("timeoutSeconds")]
        public int TimeoutSeconds { get; init; } = 30;

        [JsonPropertyName("bindLoopbackOnly")]
        public bool BindLoopbackOnly { get; init; } = true;
    }

    public sealed class RepoPathsModel
    {
        [JsonPropertyName("sourceRoot")]
        public string SourceRoot { get; init; } = "Atlas";

        [JsonPropertyName("gameRoot")]
        public string GameRoot { get; init; } = "NovaForge";

        [JsonPropertyName("toolingRoot")]
        public string ToolingRoot { get; init; } = "AtlasAI";

        [JsonPropertyName("sharedRoot")]
        public string SharedRoot { get; init; } = "Shared";

        [JsonPropertyName("docsRoot")]
        public string DocsRoot { get; init; } = "Docs";

        [JsonPropertyName("dataRoot")]
        public string DataRoot { get; init; } = "NovaForge/Data";

        [JsonPropertyName("contentRoot")]
        public string ContentRoot { get; init; } = "NovaForge/Content";

        [JsonPropertyName("scriptsRoot")]
        public string ScriptsRoot { get; init; } = "Scripts";

        [JsonPropertyName("testsRoot")]
        public string TestsRoot { get; init; } = "Tests";
    }

    public sealed class SafetySettingsModel
    {
        [JsonPropertyName("requireDryRunByDefault")]
        public bool RequireDryRunByDefault { get; init; } = true;

        [JsonPropertyName("requireSessionTokenForWrites")]
        public bool RequireSessionTokenForWrites { get; init; } = true;

        [JsonPropertyName("allowedToolActions")]
        public string[] AllowedToolActions { get; init; } = Array.Empty<string>();

        [JsonPropertyName("writeableRoots")]
        public string[] WriteableRoots { get; init; } = Array.Empty<string>();
    }

    public sealed class ProjectInfoModel
    {
        [JsonPropertyName("id")]
        public string Id { get; init; } = string.Empty;

        [JsonPropertyName("displayName")]
        public string DisplayName { get; init; } = string.Empty;

        [JsonPropertyName("version")]
        public string Version { get; init; } = string.Empty;

        [JsonPropertyName("description")]
        public string Description { get; init; } = string.Empty;

        [JsonPropertyName("repoRoot")]
        public string RepoRoot { get; init; } = string.Empty;
    }

    public sealed class NovaForgeProjectManifest
    {
        [JsonPropertyName("schemaVersion")]
        public string SchemaVersion { get; init; } = "1.0";

        [JsonPropertyName("project")]
        public ProjectInfoModel Project { get; init; } = new();

        [JsonPropertyName("capabilities")]
        public ProjectCapabilitiesModel Capabilities { get; init; } = new();

        [JsonPropertyName("buildTargets")]
        public BuildTargetModel[] BuildTargets { get; init; } = Array.Empty<BuildTargetModel>();

        [JsonPropertyName("bridge")]
        public BridgeConfigModel Bridge { get; init; } = new();

        [JsonPropertyName("repoPaths")]
        public RepoPathsModel RepoPaths { get; init; } = new();

        [JsonPropertyName("safetySettings")]
        public SafetySettingsModel SafetySettings { get; init; } = new();

        // ----------------------------------------------------------------
        // Loading
        // ----------------------------------------------------------------

        public static NovaForgeProjectManifest Load(string manifestPath)
        {
            if (!File.Exists(manifestPath))
                throw new FileNotFoundException(
                    $"NovaForge project manifest not found: {manifestPath}");

            string json = File.ReadAllText(manifestPath);

            var options = new JsonSerializerOptions
            {
                PropertyNameCaseInsensitive = true,
                ReadCommentHandling = JsonCommentHandling.Skip,
                AllowTrailingCommas = true,
            };

            return JsonSerializer.Deserialize<NovaForgeProjectManifest>(json, options)
                ?? throw new InvalidOperationException(
                    "Failed to deserialize NovaForge project manifest.");
        }

        /// <summary>
        /// Attempts to locate the manifest relative to a known repo root.
        /// </summary>
        public static string ResolveManifestPath(string repoRoot)
        {
            return Path.Combine(
                repoRoot,
                "Shared",
                "ProjectManifests",
                "novaforge.project.json");
        }
    }
}

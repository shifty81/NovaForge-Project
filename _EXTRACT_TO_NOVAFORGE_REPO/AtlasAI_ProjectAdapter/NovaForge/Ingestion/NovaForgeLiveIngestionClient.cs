// NovaForgeLiveIngestionClient.cs
// AtlasAI-side client for the NovaForge live data ingestion pipeline.
//
// Polls the NovaForge bridge at a configurable interval and delivers
// snapshots of live game-state data (economy, factions, world, players)
// to AtlasAI so the AI engine can reason about the current project state.
//
// Rules:
// - All bridge calls use the standard BridgeRequestEnvelope.
// - Snapshot delivery is event-driven: subscribe to SnapshotReceived.
// - Client is disposable; call Dispose() or use via `using`.
// - No direct game-engine references — all data flows through the bridge.

using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading;
using System.Threading.Tasks;

namespace AtlasAI.ProjectAdapters.NovaForge.Ingestion
{
    // ----------------------------------------------------------------
    // Snapshot models (mirrors C++ IngestionSnapshot types)
    // ----------------------------------------------------------------

    public sealed class EconomySnapshot
    {
        [JsonPropertyName("capturedAt")]     public string CapturedAt      { get; init; } = string.Empty;
        [JsonPropertyName("activeMarkets")]  public int    ActiveMarkets   { get; init; }
        [JsonPropertyName("openOrderCount")] public int    OpenOrderCount  { get; init; }
        [JsonPropertyName("globalPriceIndex")] public float GlobalPriceIndex { get; init; }
        [JsonPropertyName("topResourceId")]  public string TopResourceId   { get; init; } = string.Empty;
        [JsonPropertyName("topResourcePrice")] public float TopResourcePrice { get; init; }
    }

    public sealed class FactionSnapshot
    {
        [JsonPropertyName("capturedAt")]         public string CapturedAt        { get; init; } = string.Empty;
        [JsonPropertyName("factionCount")]        public int    FactionCount       { get; init; }
        [JsonPropertyName("hostilePairCount")]    public int    HostilePairCount   { get; init; }
        [JsonPropertyName("activeConflicts")]     public int    ActiveConflicts    { get; init; }
        [JsonPropertyName("dominantFactionId")]   public string DominantFactionId  { get; init; } = string.Empty;
    }

    public sealed class WorldStateSnapshot
    {
        [JsonPropertyName("capturedAt")]         public string CapturedAt        { get; init; } = string.Empty;
        [JsonPropertyName("loadedSectorCount")]  public int    LoadedSectorCount  { get; init; }
        [JsonPropertyName("totalEntityCount")]   public int    TotalEntityCount   { get; init; }
        [JsonPropertyName("activePlayerCount")]  public int    ActivePlayerCount  { get; init; }
        [JsonPropertyName("simulationRunning")]  public bool   SimulationRunning  { get; init; }
        [JsonPropertyName("activeSectorId")]     public string ActiveSectorId     { get; init; } = string.Empty;
    }

    public sealed class PlayerSystemsSnapshot
    {
        [JsonPropertyName("capturedAt")]        public string CapturedAt       { get; init; } = string.Empty;
        [JsonPropertyName("onlinePlayerCount")] public int    OnlinePlayerCount { get; init; }
        [JsonPropertyName("inCombatCount")]     public int    InCombatCount     { get; init; }
        [JsonPropertyName("dockedCount")]       public int    DockedCount       { get; init; }
        [JsonPropertyName("inSpaceCount")]      public int    InSpaceCount      { get; init; }
    }

    public sealed class IngestionSnapshot
    {
        [JsonPropertyName("sequenceId")]  public long                  SequenceId { get; init; }
        [JsonPropertyName("capturedAt")]  public string                CapturedAt { get; init; } = string.Empty;
        [JsonPropertyName("economy")]     public EconomySnapshot?      Economy    { get; init; }
        [JsonPropertyName("factions")]    public FactionSnapshot?      Factions   { get; init; }
        [JsonPropertyName("world")]       public WorldStateSnapshot?   World      { get; init; }
        [JsonPropertyName("players")]     public PlayerSystemsSnapshot? Players   { get; init; }
    }

    // ----------------------------------------------------------------
    // Client
    // ----------------------------------------------------------------

    /// <summary>
    /// Polls the NovaForge bridge for live game-state snapshots and
    /// raises <see cref="SnapshotReceived"/> on each successful delivery.
    /// </summary>
    public sealed class NovaForgeLiveIngestionClient : IDisposable
    {
        private readonly HttpClient             _http;
        private readonly IngestionStreamConfig  _config;
        private          string                 _sessionToken;
        private          bool                   _running;
        private          bool                   _disposed;
        private          int                    _consecutiveFailures;
        private          IngestionSnapshot?     _lastSnapshot;

        private static readonly JsonSerializerOptions s_jsonOpts = new()
        {
            PropertyNameCaseInsensitive = true,
        };

        // ----------------------------------------------------------------
        // Events
        // ----------------------------------------------------------------

        /// <summary>Raised every time a new snapshot is successfully received.</summary>
        public event EventHandler<IngestionSnapshot>? SnapshotReceived;

        /// <summary>Raised when the ingestion stream encounters a failure.</summary>
        public event EventHandler<string>? IngestionError;

        // ----------------------------------------------------------------
        // Construction
        // ----------------------------------------------------------------

        public NovaForgeLiveIngestionClient(
            string                 bridgeBaseUrl,
            string                 sessionToken,
            IngestionStreamConfig? config = null)
        {
            _http = new HttpClient
            {
                BaseAddress = new Uri(bridgeBaseUrl),
                Timeout     = TimeSpan.FromSeconds(10),
            };
            _sessionToken = sessionToken;
            _config       = (config ?? new IngestionStreamConfig()).Validated();
        }

        // ----------------------------------------------------------------
        // Properties
        // ----------------------------------------------------------------

        /// <summary>True while the polling loop is active.</summary>
        public bool IsRunning => _running;

        /// <summary>The most recently received snapshot, or null if none yet.</summary>
        public IngestionSnapshot? LastSnapshot => _lastSnapshot;

        /// <summary>Number of consecutive poll failures since the last success.</summary>
        public int ConsecutiveFailures => _consecutiveFailures;

        // ----------------------------------------------------------------
        // Start / Stop
        // ----------------------------------------------------------------

        /// <summary>
        /// Starts the background polling loop.
        /// Returns immediately; polling runs on the provided
        /// <paramref name="cancellationToken"/>.
        /// </summary>
        public Task StartAsync(CancellationToken cancellationToken = default)
        {
            if (_running) return Task.CompletedTask;
            _running = true;
            return PollLoopAsync(cancellationToken);
        }

        /// <summary>Stops the polling loop gracefully.</summary>
        public void Stop() => _running = false;

        // ----------------------------------------------------------------
        // Manual capture
        // ----------------------------------------------------------------

        /// <summary>
        /// Requests an immediate snapshot from the bridge outside the poll interval.
        /// </summary>
        public async Task<IngestionSnapshot?> CaptureNowAsync(
            CancellationToken cancellationToken = default)
        {
            return await FetchSnapshotAsync(cancellationToken).ConfigureAwait(false);
        }

        // ----------------------------------------------------------------
        // Polling loop
        // ----------------------------------------------------------------

        private async Task PollLoopAsync(CancellationToken ct)
        {
            while (_running && !ct.IsCancellationRequested)
            {
                var snapshot = await FetchSnapshotAsync(ct).ConfigureAwait(false);
                if (snapshot is not null)
                {
                    _consecutiveFailures = 0;
                    _lastSnapshot        = snapshot;
                    SnapshotReceived?.Invoke(this, snapshot);
                }
                else
                {
                    ++_consecutiveFailures;
                    if (_config.MaxConsecutiveFailures > 0 &&
                        _consecutiveFailures >= _config.MaxConsecutiveFailures)
                    {
                        IngestionError?.Invoke(this,
                            $"Ingestion halted after {_consecutiveFailures} consecutive failures.");
                        _running = false;
                        return;
                    }
                }

                try
                {
                    await Task.Delay(_config.PollIntervalMs, ct).ConfigureAwait(false);
                }
                catch (TaskCanceledException)
                {
                    break;
                }
            }
            _running = false;
        }

        private async Task<IngestionSnapshot?> FetchSnapshotAsync(CancellationToken ct)
        {
            try
            {
                var requestPayload = new
                {
                    includeEconomy       = _config.IncludeEconomy,
                    includeFactions      = _config.IncludeFactions,
                    includeWorldState    = _config.IncludeWorldState,
                    includePlayerSystems = _config.IncludePlayerSystems,
                };

                var envelope = new
                {
                    protocolVersion = "1.0",
                    requestId       = Guid.NewGuid().ToString(),
                    sessionId       = _sessionToken,
                    service         = "IngestionService",
                    operation       = "CaptureSnapshot",
                    timestampUtc    = DateTime.UtcNow.ToString("o"),
                    payload         = requestPayload,
                };

                string json    = JsonSerializer.Serialize(envelope, s_jsonOpts);
                var    content = new StringContent(json, Encoding.UTF8, "application/json");

                using var resp = await _http
                    .PostAsync("/ingestion/snapshot", content, ct)
                    .ConfigureAwait(false);

                if (!resp.IsSuccessStatusCode) return null;

                string body = await resp.Content.ReadAsStringAsync(ct).ConfigureAwait(false);
                return JsonSerializer.Deserialize<IngestionSnapshot>(body, s_jsonOpts);
            }
            catch
            {
                return null;
            }
        }

        // ----------------------------------------------------------------
        // Disposal
        // ----------------------------------------------------------------

        public void Dispose()
        {
            if (_disposed) return;
            _disposed = true;
            _running  = false;
            _http.Dispose();
        }
    }
}

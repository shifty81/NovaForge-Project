// IngestionStreamConfig.cs
// Configuration for the NovaForge live ingestion stream consumed by AtlasAI.
//
// Controls which subsystem snapshots are requested, the polling interval,
// and the bridge endpoint to query.

using System;
using System.Text.Json.Serialization;

namespace AtlasAI.ProjectAdapters.NovaForge.Ingestion
{
    /// <summary>
    /// Configuration supplied to <see cref="NovaForgeLiveIngestionClient"/> at
    /// construction or reconfiguration time.
    /// </summary>
    public sealed class IngestionStreamConfig
    {
        // ----------------------------------------------------------------
        // Subsystem toggles
        // ----------------------------------------------------------------

        /// <summary>Include economy snapshots (markets, prices, orders).</summary>
        [JsonPropertyName("includeEconomy")]
        public bool IncludeEconomy { get; init; } = true;

        /// <summary>Include faction snapshots (standings, conflicts).</summary>
        [JsonPropertyName("includeFactions")]
        public bool IncludeFactions { get; init; } = true;

        /// <summary>Include world-state snapshots (sectors, entity counts).</summary>
        [JsonPropertyName("includeWorldState")]
        public bool IncludeWorldState { get; init; } = true;

        /// <summary>Include player-systems snapshots (online, docked, in-combat).</summary>
        [JsonPropertyName("includePlayerSystems")]
        public bool IncludePlayerSystems { get; init; } = true;

        // ----------------------------------------------------------------
        // Polling
        // ----------------------------------------------------------------

        /// <summary>
        /// How often AtlasAI polls the NovaForge bridge for a new snapshot.
        /// Minimum is 1 second to avoid hammering the bridge.
        /// </summary>
        [JsonPropertyName("pollIntervalMs")]
        public int PollIntervalMs { get; init; } = 5000;

        // ----------------------------------------------------------------
        // Safety
        // ----------------------------------------------------------------

        /// <summary>
        /// Maximum consecutive failed polls before the client backs off.
        /// Zero means no limit (not recommended in production).
        /// </summary>
        [JsonPropertyName("maxConsecutiveFailures")]
        public int MaxConsecutiveFailures { get; init; } = 5;

        // ----------------------------------------------------------------
        // Validation
        // ----------------------------------------------------------------

        /// <summary>
        /// Returns a copy of this config with all values clamped to valid ranges.
        /// </summary>
        public IngestionStreamConfig Validated() =>
            new IngestionStreamConfig
            {
                IncludeEconomy        = IncludeEconomy,
                IncludeFactions       = IncludeFactions,
                IncludeWorldState     = IncludeWorldState,
                IncludePlayerSystems  = IncludePlayerSystems,
                PollIntervalMs        = Math.Max(1000, PollIntervalMs),
                MaxConsecutiveFailures = Math.Max(0, MaxConsecutiveFailures),
            };
    }
}

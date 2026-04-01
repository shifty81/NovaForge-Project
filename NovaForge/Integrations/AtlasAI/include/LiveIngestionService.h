// LiveIngestionService.h
// NovaForge live data ingestion service for AtlasAI.
//
// Streams live game-state snapshots (economy, factions, world) to AtlasAI so
// the AI engine can reason about the current project state in real time.
//
// Rules:
// - Must not hold game-thread locks longer than necessary for a snapshot.
// - All data is copied out; AtlasAI never holds references into engine memory.
// - Transport is push-based: the service calls a registered callback whenever
//   a new snapshot is ready.  AtlasAI pulls via polling when callbacks are not
//   wired up.
// - Guarded by NOVAFORGE_ENABLE_ATLASAI_INTEGRATION.

#pragma once

#include <AtlasBridgeTypes.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace NovaForge::Integration::AtlasAI
{

// ============================================================
// Ingestion configuration
// ============================================================

struct IngestionConfig
{
    uint32_t    snapshotIntervalMs    = 5000;  ///< How often to capture a snapshot (ms)
    bool        includeEconomy        = true;
    bool        includeFactions       = true;
    bool        includeWorldState     = true;
    bool        includePlayerSystems  = true;
    uint32_t    maxEntitiesPerSnapshot = 1000;
};

// ============================================================
// Snapshot types (plain-data, no engine references)
// ============================================================

struct EconomySnapshot
{
    std::string  capturedAt;
    uint32_t     activeMarkets      = 0;
    uint32_t     openOrderCount     = 0;
    float        globalPriceIndex   = 1.0f;   ///< relative to baseline
    std::string  topResourceId;               ///< most-traded resource
    float        topResourcePrice   = 0.0f;
};

struct FactionSnapshot
{
    std::string  capturedAt;
    uint32_t     factionCount       = 0;
    uint32_t     hostilePairCount   = 0;      ///< factions at war
    uint32_t     activeConflicts    = 0;
    std::string  dominantFactionId;
};

struct WorldStateSnapshot
{
    std::string  capturedAt;
    uint32_t     loadedSectorCount  = 0;
    uint32_t     totalEntityCount   = 0;
    uint32_t     activePlayerCount  = 0;
    bool         simulationRunning  = false;
    std::string  activeSectorId;
};

struct PlayerSystemsSnapshot
{
    std::string  capturedAt;
    uint32_t     onlinePlayerCount  = 0;
    uint32_t     inCombatCount      = 0;
    uint32_t     dockedCount        = 0;
    uint32_t     inSpaceCount       = 0;
};

/// Full ingestion snapshot bundling all subsystem captures.
struct IngestionSnapshot
{
    uint64_t             sequenceId    = 0;
    std::string          capturedAt;
    EconomySnapshot      economy;
    FactionSnapshot      factions;
    WorldStateSnapshot   world;
    PlayerSystemsSnapshot players;
};

// ============================================================
// Callback types
// ============================================================

using IngestionSnapshotCallback = std::function<void(const IngestionSnapshot&)>;

// ============================================================
// LiveIngestionService
// ============================================================

class LiveIngestionService
{
public:
    LiveIngestionService();
    ~LiveIngestionService();

    // --------------------------------------------------------
    // Lifecycle
    // --------------------------------------------------------

    /// Configure and start the ingestion service.
    bool start(const IngestionConfig& config);

    /// Stop the ingestion service and flush any pending data.
    void stop();

    bool isRunning() const;

    // --------------------------------------------------------
    // Snapshot delivery
    // --------------------------------------------------------

    /// Register a callback invoked on every new snapshot.
    void setSnapshotCallback(IngestionSnapshotCallback callback);

    /// Manually trigger an immediate snapshot capture (for testing).
    IngestionSnapshot captureNow();

    // --------------------------------------------------------
    // Last-known snapshots (pull model)
    // --------------------------------------------------------

    EconomySnapshot       getLastEconomySnapshot()    const;
    FactionSnapshot       getLastFactionSnapshot()    const;
    WorldStateSnapshot    getLastWorldStateSnapshot() const;
    PlayerSystemsSnapshot getLastPlayerSnapshot()     const;
    IngestionSnapshot     getLastSnapshot()           const;

    uint64_t snapshotSequence() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace NovaForge::Integration::AtlasAI

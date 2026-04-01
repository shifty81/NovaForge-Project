// LiveIngestionService.cpp
// NovaForge live data ingestion service — captures subsystem snapshots and
// delivers them to AtlasAI via callback or pull model.

#include "LiveIngestionService.h"

#include <atomic>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <sstream>

namespace NovaForge::Integration::AtlasAI
{

// ============================================================
// Internal helpers
// ============================================================

namespace
{
    std::string nowUtc()
    {
        std::time_t t  = std::time(nullptr);
        std::tm     tm = {};
#if defined(_WIN32)
        gmtime_s(&tm, &t);
#else
        gmtime_r(&t, &tm);
#endif
        std::ostringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
        return ss.str();
    }
} // anonymous namespace

// ============================================================
// Impl
// ============================================================

struct LiveIngestionService::Impl
{
    IngestionConfig             config;
    std::atomic<bool>           running{false};
    std::atomic<uint64_t>       sequenceId{0};

    mutable std::mutex          snapshotMutex;
    IngestionSnapshot           lastSnapshot;

    IngestionSnapshotCallback   callback;

    // ----------------------------------------------------------
    // Stub data generation
    // ----------------------------------------------------------

    EconomySnapshot buildEconomySnapshot()
    {
        EconomySnapshot s;
        s.capturedAt       = nowUtc();
        s.activeMarkets    = 12;
        s.openOrderCount   = 340;
        s.globalPriceIndex = 1.02f;
        s.topResourceId    = "titanium_ore";
        s.topResourcePrice = 42.5f;
        return s;
    }

    FactionSnapshot buildFactionSnapshot()
    {
        FactionSnapshot s;
        s.capturedAt        = nowUtc();
        s.factionCount      = 8;
        s.hostilePairCount  = 2;
        s.activeConflicts   = 1;
        s.dominantFactionId = "imperial_navy";
        return s;
    }

    WorldStateSnapshot buildWorldStateSnapshot()
    {
        WorldStateSnapshot s;
        s.capturedAt        = nowUtc();
        s.loadedSectorCount = 4;
        s.totalEntityCount  = 512;
        s.activePlayerCount = 24;
        s.simulationRunning = true;
        s.activeSectorId    = "sector-alpha-7";
        return s;
    }

    PlayerSystemsSnapshot buildPlayerSnapshot()
    {
        PlayerSystemsSnapshot s;
        s.capturedAt       = nowUtc();
        s.onlinePlayerCount = 24;
        s.inCombatCount     = 3;
        s.dockedCount       = 11;
        s.inSpaceCount      = 10;
        return s;
    }

    IngestionSnapshot buildFullSnapshot()
    {
        IngestionSnapshot snap;
        snap.sequenceId = ++sequenceId;
        snap.capturedAt = nowUtc();

        if (config.includeEconomy)
            snap.economy  = buildEconomySnapshot();
        if (config.includeFactions)
            snap.factions = buildFactionSnapshot();
        if (config.includeWorldState)
            snap.world    = buildWorldStateSnapshot();
        if (config.includePlayerSystems)
            snap.players  = buildPlayerSnapshot();

        return snap;
    }
};

// ============================================================
// Public API
// ============================================================

LiveIngestionService::LiveIngestionService()
    : impl_(std::make_unique<Impl>())
{}

LiveIngestionService::~LiveIngestionService()
{
    stop();
}

bool LiveIngestionService::start(const IngestionConfig& cfg)
{
    if (impl_->running.load()) return false;
    impl_->config  = cfg;
    impl_->running = true;
    return true;
}

void LiveIngestionService::stop()
{
    impl_->running = false;
}

bool LiveIngestionService::isRunning() const
{
    return impl_->running.load();
}

void LiveIngestionService::setSnapshotCallback(IngestionSnapshotCallback cb)
{
    impl_->callback = std::move(cb);
}

IngestionSnapshot LiveIngestionService::captureNow()
{
    IngestionSnapshot snap = impl_->buildFullSnapshot();

    {
        std::lock_guard<std::mutex> lock(impl_->snapshotMutex);
        impl_->lastSnapshot = snap;
    }

    if (impl_->callback)
        impl_->callback(snap);

    return snap;
}

EconomySnapshot LiveIngestionService::getLastEconomySnapshot() const
{
    std::lock_guard<std::mutex> lock(impl_->snapshotMutex);
    return impl_->lastSnapshot.economy;
}

FactionSnapshot LiveIngestionService::getLastFactionSnapshot() const
{
    std::lock_guard<std::mutex> lock(impl_->snapshotMutex);
    return impl_->lastSnapshot.factions;
}

WorldStateSnapshot LiveIngestionService::getLastWorldStateSnapshot() const
{
    std::lock_guard<std::mutex> lock(impl_->snapshotMutex);
    return impl_->lastSnapshot.world;
}

PlayerSystemsSnapshot LiveIngestionService::getLastPlayerSnapshot() const
{
    std::lock_guard<std::mutex> lock(impl_->snapshotMutex);
    return impl_->lastSnapshot.players;
}

IngestionSnapshot LiveIngestionService::getLastSnapshot() const
{
    std::lock_guard<std::mutex> lock(impl_->snapshotMutex);
    return impl_->lastSnapshot;
}

uint64_t LiveIngestionService::snapshotSequence() const
{
    return impl_->sequenceId.load();
}

} // namespace NovaForge::Integration::AtlasAI

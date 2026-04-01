// BridgeAuditLogger.h
// Structured audit logger for the NovaForge AtlasAI bridge service.
//
// Every bridge operation (session connect, build request, tool action, etc.)
// MUST be logged here so that all AI-triggered activity is auditable.
//
// Rules:
// - Must not depend on gameplay or engine internals.
// - Must be usable from AtlasBridgeService without a running HTTP server.
// - Log entries are written synchronously to avoid ordering ambiguity.

#pragma once

#include <AtlasBridgeTypes.h>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace NovaForge::Integration::AtlasAI
{

// ============================================================
// Log sink callback type
// ============================================================

using AuditLogSink = std::function<void(const ::Atlas::Bridge::AuditLogEntry&)>;

// ============================================================
// BridgeAuditLogger
// ============================================================

class BridgeAuditLogger
{
public:
    BridgeAuditLogger();
    ~BridgeAuditLogger();

    // --------------------------------------------------------
    // Configuration
    // --------------------------------------------------------

    /// Sets an optional file path for persistent audit log output.
    /// If not set, entries are held in memory only.
    void setLogFilePath(const std::string& path);

    /// Adds a sink callback to receive each audit entry as it is written.
    void addSink(AuditLogSink sink);

    // --------------------------------------------------------
    // Writing entries
    // --------------------------------------------------------

    /// Logs a bridge operation result.
    void log(const ::Atlas::Bridge::AuditLogEntry& entry);

    /// Convenience overload: logs a simple operation success/failure.
    void log(
        const std::string& requestId,
        const std::string& sessionId,
        const std::string& service,
        const std::string& operation,
        bool               success,
        const std::string& summary,
        bool               wasDryRun  = false,
        const std::string& failReason = {});

    // --------------------------------------------------------
    // Retrieval
    // --------------------------------------------------------

    /// Returns all in-memory log entries (most recent last).
    std::vector<::Atlas::Bridge::AuditLogEntry> getEntries() const;

    /// Returns the last N entries.
    std::vector<::Atlas::Bridge::AuditLogEntry> getRecentEntries(
        std::size_t count) const;

    /// Clears the in-memory entry buffer.
    void clearEntries();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    static std::string utcNowIso8601();
};

} // namespace NovaForge::Integration::AtlasAI

// BridgeAuditLogger.cpp
// Implementation of structured audit logging for the bridge service.

#include "BridgeAuditLogger.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace NovaForge::Integration::AtlasAI
{

// ============================================================
// Internal state
// ============================================================

struct BridgeAuditLogger::Impl
{
    std::string                                          logFilePath;
    std::vector<::Atlas::Bridge::AuditLogEntry>        entries;
    std::vector<AuditLogSink>                            sinks;
    static constexpr std::size_t                         kMaxInMemoryEntries = 1000;
};

// ============================================================
// Construction
// ============================================================

BridgeAuditLogger::BridgeAuditLogger()
    : m_impl(std::make_unique<Impl>())
{
}

BridgeAuditLogger::~BridgeAuditLogger() = default;

// ============================================================
// Configuration
// ============================================================

void BridgeAuditLogger::setLogFilePath(const std::string& path)
{
    m_impl->logFilePath = path;
}

void BridgeAuditLogger::addSink(AuditLogSink sink)
{
    m_impl->sinks.push_back(std::move(sink));
}

// ============================================================
// Writing entries
// ============================================================

void BridgeAuditLogger::log(const ::Atlas::Bridge::AuditLogEntry& entry)
{
    // Cap in-memory buffer
    if (m_impl->entries.size() >= Impl::kMaxInMemoryEntries)
        m_impl->entries.erase(m_impl->entries.begin());

    m_impl->entries.push_back(entry);

    // Write to file if configured
    if (!m_impl->logFilePath.empty())
    {
        std::ofstream file(m_impl->logFilePath, std::ios::app);
        if (file.is_open())
        {
            file << '[' << entry.timestampUtc << ']'
                 << ' ' << entry.service << '.' << entry.operation
                 << ' ' << (entry.success ? "OK" : "FAIL")
                 << " req=" << entry.requestId
                 << " session=" << entry.sessionId;

            if (entry.wasDryRun)
                file << " [DRY RUN]";

            if (!entry.summary.empty())
                file << " | " << entry.summary;

            if (!entry.success && !entry.failReason.empty())
                file << " | reason=" << entry.failReason;

            file << '\n';
        }
    }

    // Notify sinks
    for (const auto& sink : m_impl->sinks)
        sink(entry);
}

void BridgeAuditLogger::log(
    const std::string& requestId,
    const std::string& sessionId,
    const std::string& service,
    const std::string& operation,
    bool               success,
    const std::string& summary,
    bool               wasDryRun,
    const std::string& failReason)
{
    ::Atlas::Bridge::AuditLogEntry entry;
    entry.timestampUtc = utcNowIso8601();
    entry.requestId    = requestId;
    entry.sessionId    = sessionId;
    entry.service      = service;
    entry.operation    = operation;
    entry.success      = success;
    entry.summary      = summary;
    entry.wasDryRun    = wasDryRun;
    entry.failReason   = failReason;
    log(entry);
}

// ============================================================
// Retrieval
// ============================================================

std::vector<::Atlas::Bridge::AuditLogEntry> BridgeAuditLogger::getEntries() const
{
    return m_impl->entries;
}

std::vector<::Atlas::Bridge::AuditLogEntry> BridgeAuditLogger::getRecentEntries(
    std::size_t count) const
{
    if (count >= m_impl->entries.size())
        return m_impl->entries;

    return std::vector<::Atlas::Bridge::AuditLogEntry>(
        m_impl->entries.end() - static_cast<std::ptrdiff_t>(count),
        m_impl->entries.end());
}

void BridgeAuditLogger::clearEntries()
{
    m_impl->entries.clear();
}

// ============================================================
// Helpers
// ============================================================

std::string BridgeAuditLogger::utcNowIso8601()
{
    auto now = std::chrono::system_clock::now();
    auto tt  = std::chrono::system_clock::to_time_t(now);
    std::tm utc{};
#if defined(_WIN32)
    gmtime_s(&utc, &tt);
#else
    gmtime_r(&tt, &utc);
#endif
    std::ostringstream oss;
    oss << std::put_time(&utc, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

} // namespace NovaForge::Integration::AtlasAI

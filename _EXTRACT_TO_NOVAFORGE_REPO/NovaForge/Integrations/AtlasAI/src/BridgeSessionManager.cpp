// BridgeSessionManager.cpp
// Implementation of bridge session management.

#include "BridgeSessionManager.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>
#include <unordered_map>

namespace NovaForge::Integration::AtlasAI
{

// ============================================================
// Internal state
// ============================================================

struct BridgeSessionManager::Impl
{
    std::unordered_map<std::string, BridgeSession> sessions;
    std::mt19937_64 rng{std::random_device{}()};
};

// ============================================================
// Construction
// ============================================================

BridgeSessionManager::BridgeSessionManager()
    : m_impl(std::make_unique<Impl>())
{
}

BridgeSessionManager::~BridgeSessionManager() = default;

// ============================================================
// Session creation
// ============================================================

::Atlas::Bridge::SessionConnectResponse BridgeSessionManager::createSession(
    const ::Atlas::Bridge::SessionConnectRequest& request,
    const std::string& serverVersion)
{
    ::Atlas::Bridge::SessionConnectResponse response;

    if (request.protocolVersion != ::Atlas::Bridge::kProtocolVersion)
    {
        response.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::VersionMismatch;
        response.result.message   =
            "Protocol version mismatch: expected "
            + std::string(::Atlas::Bridge::kProtocolVersion)
            + ", got " + request.protocolVersion;
        return response;
    }

    BridgeSession session;
    session.sessionToken  = generateToken();
    session.projectId     = request.projectId;
    session.createdAtUtc  = utcNowIso8601();
    session.writeEnabled  = false; // write capability must be explicitly granted
    session.active        = true;

    m_impl->sessions[session.sessionToken] = session;

    response.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
    response.result.message   = "Session created";
    response.sessionToken     = session.sessionToken;
    response.serverVersion    = serverVersion;
    response.projectId        = request.projectId;
    response.writeEnabled     = session.writeEnabled;

    return response;
}

// ============================================================
// Token validation
// ============================================================

bool BridgeSessionManager::validateToken(const std::string& token) const
{
    auto it = m_impl->sessions.find(token);
    return it != m_impl->sessions.end() && it->second.active;
}

bool BridgeSessionManager::isWriteAuthorized(const std::string& token) const
{
    auto it = m_impl->sessions.find(token);
    return it != m_impl->sessions.end()
        && it->second.active
        && it->second.writeEnabled;
}

// ============================================================
// Session access
// ============================================================

BridgeSession BridgeSessionManager::getSession(const std::string& token) const
{
    auto it = m_impl->sessions.find(token);
    if (it != m_impl->sessions.end())
        return it->second;
    return {};
}

// ============================================================
// Revocation
// ============================================================

void BridgeSessionManager::revokeSession(const std::string& token)
{
    auto it = m_impl->sessions.find(token);
    if (it != m_impl->sessions.end())
        it->second.active = false;
}

void BridgeSessionManager::revokeAll()
{
    for (auto& [token, session] : m_impl->sessions)
        session.active = false;
}

size_t BridgeSessionManager::activeSessionCount() const
{
    size_t count = 0;
    for (const auto& [token, session] : m_impl->sessions)
        if (session.active) ++count;
    return count;
}

// ============================================================
// Helpers
// ============================================================

std::string BridgeSessionManager::generateToken()
{
    // Generate a UUID v4-style hex string using the persistent instance RNG
    std::uniform_int_distribution<uint64_t> dist;

    uint64_t a = dist(m_impl->rng);
    uint64_t b = dist(m_impl->rng);

    // Set UUID v4 version field (bits 12-15 of octet 6 = 0100 = 4) per RFC 4122 §4.4
    a = (a & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;
    // Set UUID variant field (bits 62-63 of octet 8 = 10) per RFC 4122 §4.1.1
    b = (b & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;

    std::ostringstream oss;
    oss << std::hex << std::setfill('0')
        << std::setw(8)  << ((a >> 32) & 0xFFFFFFFF) << '-'
        << std::setw(4)  << ((a >> 16) & 0xFFFF)     << '-'
        << std::setw(4)  << (a & 0xFFFF)              << '-'
        << std::setw(4)  << ((b >> 48) & 0xFFFF)      << '-'
        << std::setw(12) << (b & 0xFFFFFFFFFFFFULL);
    return oss.str();
}

std::string BridgeSessionManager::utcNowIso8601()
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

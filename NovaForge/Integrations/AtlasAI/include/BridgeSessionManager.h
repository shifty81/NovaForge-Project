// BridgeSessionManager.h
// Manages bridge session tokens for the NovaForge AtlasAI bridge service.
//
// Responsibilities:
// - Generate unique session tokens on connect
// - Validate tokens on subsequent requests
// - Track session write capability
// - Revoke sessions on disconnect or timeout

#pragma once

#include <AtlasBridgeTypes.h>
#include <memory>
#include <string>

namespace NovaForge::Integration::AtlasAI
{

// ============================================================
// Session record
// ============================================================

struct BridgeSession
{
    std::string sessionToken;
    std::string projectId;
    std::string createdAtUtc;
    bool        writeEnabled = false;
    bool        active       = true;
};

// ============================================================
// BridgeSessionManager
// ============================================================

class BridgeSessionManager
{
public:
    BridgeSessionManager();
    ~BridgeSessionManager();

    /// Creates a new session and returns the session token.
    ::Atlas::Bridge::SessionConnectResponse createSession(
        const ::Atlas::Bridge::SessionConnectRequest& request,
        const std::string& serverVersion);

    /// Returns true if the token is valid and the session is active.
    bool validateToken(const std::string& token) const;

    /// Returns true if the token is valid and write-enabled.
    bool isWriteAuthorized(const std::string& token) const;

    /// Revokes a session, invalidating its token.
    void revokeSession(const std::string& token);

    /// Returns a copy of the session record, or a default if not found.
    BridgeSession getSession(const std::string& token) const;

    /// Revokes all active sessions.
    void revokeAll();

    /// Returns the number of currently active sessions.
    size_t activeSessionCount() const;

    /// Returns the current UTC time formatted as ISO 8601.
    static std::string utcNowIso8601();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    std::string generateToken();
};

} // namespace NovaForge::Integration::AtlasAI

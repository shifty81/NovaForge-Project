// NovaForgeSession.h
// Game-side session lifecycle management.
//
// Epic 5 / Task 5.1 — Modular startup: session component
//
// Owns the current bridge session token and connection state.
// All bridge-authenticated operations should call isConnected() before use.

#pragma once

#include <cstdint>
#include <string>

namespace NovaForge::App
{

// ============================================================
// Session state machine
// ============================================================

enum class SessionState : uint8_t
{
    Disconnected  = 0,
    Connecting    = 1,
    Connected     = 2,
    Disconnecting = 3,
};

// ============================================================
// NovaForgeSession
// ============================================================

class NovaForgeSession
{
public:
    NovaForgeSession() = default;

    // --------------------------------------------------------
    // State
    // --------------------------------------------------------
    SessionState state()       const;
    bool         isConnected() const;

    // --------------------------------------------------------
    // Token management
    // --------------------------------------------------------
    const std::string& sessionToken() const;

    /// Called by bootstrap when a session connect response is received.
    void onConnected(std::string token);

    /// Called by bootstrap when disconnection completes.
    void onDisconnected();

    /// Marks the session as connecting (in-flight handshake).
    void onConnecting();

    /// Marks the session as disconnecting (in-flight teardown).
    void onDisconnecting();

    // --------------------------------------------------------
    // Reset
    // --------------------------------------------------------
    void reset();

private:
    SessionState m_state = SessionState::Disconnected;
    std::string  m_token;
};

} // namespace NovaForge::App

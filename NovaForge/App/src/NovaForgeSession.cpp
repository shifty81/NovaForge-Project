// NovaForgeSession.cpp
// Game-side session lifecycle implementation.

#include "NovaForgeSession.h"

namespace NovaForge::App
{

SessionState NovaForgeSession::state() const
{
    return m_state;
}

bool NovaForgeSession::isConnected() const
{
    return m_state == SessionState::Connected;
}

const std::string& NovaForgeSession::sessionToken() const
{
    return m_token;
}

void NovaForgeSession::onConnecting()
{
    m_state = SessionState::Connecting;
    m_token.clear();
}

void NovaForgeSession::onConnected(std::string token)
{
    m_token = std::move(token);
    m_state = SessionState::Connected;
}

void NovaForgeSession::onDisconnecting()
{
    m_state = SessionState::Disconnecting;
}

void NovaForgeSession::onDisconnected()
{
    m_state = SessionState::Disconnected;
    m_token.clear();
}

void NovaForgeSession::reset()
{
    m_state = SessionState::Disconnected;
    m_token.clear();
}

} // namespace NovaForge::App

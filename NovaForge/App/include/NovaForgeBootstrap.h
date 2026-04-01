// NovaForgeBootstrap.h
// Startup orchestration for NovaForge.
//
// Epic 5 / Task 5.1 — Modular startup / bootstrap
//
// Responsibilities:
// - Create and validate the project context (paths, metadata)
// - Start the AtlasAI bridge service when the integration is enabled
// - Establish a bridge session and store the token in NovaForgeSession
// - Provide a clean shutdown path that reverses all of the above
//
// Rules:
// - Bootstrap must not contain gameplay logic.
// - Bridge startup is conditional on NOVAFORGE_BRIDGE_SERVER_ENABLED.
// - Bootstrap owns the bridge service lifetime.

#pragma once

#include "NovaForgeProjectContext.h"
#include "NovaForgeSession.h"

#include <functional>
#include <memory>
#include <string>

// Forward declaration — avoid pulling the full bridge header into game code
// when AtlasAI integration is disabled.
#if defined(NOVAFORGE_BRIDGE_SERVER_ENABLED)
namespace NovaForge::Integration::AtlasAI { class AtlasBridgeService; }
namespace NovaForge::Integration::AtlasAI { class BridgeAuditLogger; }
#endif

namespace NovaForge::App
{

// ============================================================
// Bootstrap result
// ============================================================

struct BootstrapResult
{
    bool        success = false;
    std::string message;
};

// ============================================================
// Bootstrap configuration
// ============================================================

struct BootstrapConfig
{
    /// Absolute path to the repository root.
    std::string repoRoot;

    /// Override for project context (leave empty to use defaults).
    ProjectContextConfig contextConfig;

    /// If true, attempt to start the AtlasAI bridge service.
    bool startBridgeService = true;

    /// Log callback forwarded to the bridge service.
    std::function<void(const std::string&)> logCallback;
};

// ============================================================
// NovaForgeBootstrap
// ============================================================

class NovaForgeBootstrap
{
public:
    NovaForgeBootstrap();
    ~NovaForgeBootstrap();

    // --------------------------------------------------------
    // Startup / shutdown
    // --------------------------------------------------------

    /// Runs the full startup sequence and returns a result.
    BootstrapResult run(const BootstrapConfig& config);

    /// Shuts down all services started by run().
    void shutdown();

    // --------------------------------------------------------
    // Accessors (only valid after a successful run())
    // --------------------------------------------------------

    /// Returns the project context, or nullptr if not yet bootstrapped.
    const NovaForgeProjectContext* projectContext() const;

    /// Returns the active session, or nullptr if not bootstrapped.
    const NovaForgeSession* session() const;

    bool isRunning() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace NovaForge::App

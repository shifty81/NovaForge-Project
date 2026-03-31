// NovaForgeBootstrap.cpp
// Startup orchestration implementation.

#include "NovaForgeBootstrap.h"

#if defined(NOVAFORGE_BRIDGE_SERVER_ENABLED)
#   include "AtlasBridgeService.h"
#   include "BridgeAuditLogger.h"
#endif

#include <memory>

namespace NovaForge::App
{

// ============================================================
// Internal state
// ============================================================

struct NovaForgeBootstrap::Impl
{
    std::unique_ptr<NovaForgeProjectContext> context;
    std::unique_ptr<NovaForgeSession>        session;
    bool                                     running = false;

#if defined(NOVAFORGE_BRIDGE_SERVER_ENABLED)
    std::unique_ptr<Integration::AtlasAI::BridgeAuditLogger>  auditLogger;
    std::unique_ptr<Integration::AtlasAI::AtlasBridgeService> bridgeService;
#endif
};

// ============================================================
// Construction
// ============================================================

NovaForgeBootstrap::NovaForgeBootstrap()
    : m_impl(std::make_unique<Impl>())
{
}

NovaForgeBootstrap::~NovaForgeBootstrap()
{
    shutdown();
}

// ============================================================
// Startup
// ============================================================

BootstrapResult NovaForgeBootstrap::run(const BootstrapConfig& config)
{
    if (m_impl->running)
        return { false, "Bootstrap is already running" };

    // -- Step 1: build project context ----------------------
    ProjectContextConfig ctxCfg = config.contextConfig;
    if (ctxCfg.repoRoot.empty())
        ctxCfg.repoRoot = config.repoRoot;

    if (ctxCfg.repoRoot.empty())
        return { false, "repoRoot must not be empty" };

    m_impl->context = std::make_unique<NovaForgeProjectContext>(ctxCfg);

    if (!m_impl->context->isValid())
        return { false, "Project context validation failed" };

    // -- Step 2: initialise session -------------------------
    m_impl->session = std::make_unique<NovaForgeSession>();

    // -- Step 3: start bridge service (conditional) --------
#if defined(NOVAFORGE_BRIDGE_SERVER_ENABLED)
    if (config.startBridgeService)
    {
        m_impl->auditLogger    = std::make_unique<Integration::AtlasAI::BridgeAuditLogger>();
        m_impl->bridgeService  = std::make_unique<Integration::AtlasAI::AtlasBridgeService>();

        if (config.logCallback)
            m_impl->bridgeService->setLogCallback(config.logCallback);

        m_impl->bridgeService->setAuditLogger(m_impl->auditLogger.get());

        Integration::AtlasAI::BridgeServiceConfig svcCfg;
        if (!m_impl->bridgeService->start(svcCfg))
            return { false, "Failed to start AtlasAI bridge service" };

        // -- Step 4: connect session -------------------------
        m_impl->session->onConnecting();

        ::Atlas::Bridge::SessionConnectRequest req;
        req.projectId     = m_impl->context->projectId();
        req.clientVersion = m_impl->context->version();

        auto resp = m_impl->bridgeService->connectSession(req);
        if (!resp.result.succeeded())
        {
            m_impl->session->onDisconnected();
            m_impl->bridgeService->stop();
            return { false, "Session connect failed: " + resp.result.message };
        }

        m_impl->session->onConnected(resp.sessionToken);
    }
#endif

    m_impl->running = true;
    return { true, "Bootstrap complete" };
}

// ============================================================
// Shutdown
// ============================================================

void NovaForgeBootstrap::shutdown()
{
    if (!m_impl->running)
        return;

#if defined(NOVAFORGE_BRIDGE_SERVER_ENABLED)
    if (m_impl->session && m_impl->session->isConnected() && m_impl->bridgeService)
    {
        m_impl->session->onDisconnecting();
        m_impl->bridgeService->disconnectSession(m_impl->session->sessionToken());
        m_impl->session->onDisconnected();
    }

    if (m_impl->bridgeService)
    {
        m_impl->bridgeService->stop();
        m_impl->bridgeService.reset();
    }
    m_impl->auditLogger.reset();
#endif

    m_impl->session.reset();
    m_impl->context.reset();
    m_impl->running = false;
}

// ============================================================
// Accessors
// ============================================================

const NovaForgeProjectContext* NovaForgeBootstrap::projectContext() const
{
    return m_impl->context.get();
}

const NovaForgeSession* NovaForgeBootstrap::session() const
{
    return m_impl->session.get();
}

bool NovaForgeBootstrap::isRunning() const
{
    return m_impl->running;
}

} // namespace NovaForge::App

// AtlasBridgeService.h
// NovaForge-side bridge service that exposes safe editor and build operations to AtlasAI.
//
// Rules:
// - This file must not expose arbitrary engine internals.
// - All operations must be editor-safe and explicitly permissioned.
// - Write operations default to dry-run unless explicitly overridden.
// - This service is guarded by NOVAFORGE_ENABLE_ATLASAI_INTEGRATION.

#pragma once

#include <AtlasBridgeTypes.h>
#include "BridgeAuditLogger.h"
#include <functional>
#include <memory>
#include <string>

namespace NovaForge::Integration::AtlasAI
{

// ============================================================
// Service configuration
// ============================================================

struct BridgeServiceConfig
{
    uint16_t    restPort          = ::Atlas::Bridge::kDefaultRestPort;
    uint16_t    wsPort            = ::Atlas::Bridge::kDefaultWsPort;
    bool        bindLoopbackOnly  = true;
    uint32_t    timeoutSeconds    = 30;
};

// ============================================================
// Log callback
// ============================================================

using BridgeLogCallback = std::function<void(const std::string& message)>;

// ============================================================
// AtlasBridgeService
// ============================================================

class AtlasBridgeService
{
public:
    AtlasBridgeService();
    ~AtlasBridgeService();

    // --------------------------------------------------------
    // Lifecycle
    // --------------------------------------------------------
    bool start(const BridgeServiceConfig& config);
    void stop();
    bool isRunning() const;

    // --------------------------------------------------------
    // Logging and audit
    // --------------------------------------------------------
    void setLogCallback(BridgeLogCallback callback);
    void setAuditLogger(BridgeAuditLogger* logger);

    // --------------------------------------------------------
    // Session management (Epic 4 / Task 4.1)
    // --------------------------------------------------------

    /// Establishes a new bridge session and returns a session token.
    ::Atlas::Bridge::SessionConnectResponse connectSession(
        const ::Atlas::Bridge::SessionConnectRequest& request);

    /// Disconnects and invalidates the session for the given token.
    ::Atlas::Bridge::BridgeResult disconnectSession(
        const std::string& sessionToken);

    // --------------------------------------------------------
    // Project info endpoint  GET /project/info  (Task 4.1)
    // --------------------------------------------------------
    ::Atlas::Bridge::ProjectInfo getProjectInfo(
        const std::string& sessionToken) const;

    // --------------------------------------------------------
    // Build endpoint  POST /build/run  (Task 4.3)
    // --------------------------------------------------------
    ::Atlas::Bridge::BuildResult runBuild(
        const std::string&                    sessionToken,
        const ::Atlas::Bridge::BuildTarget& target);

    // --------------------------------------------------------
    // Editor state endpoint  GET /editor/selection  (Task 4.2)
    // --------------------------------------------------------
    ::Atlas::Bridge::EditorSelectionSnapshot getEditorSelection(
        const std::string& sessionToken) const;

    // --------------------------------------------------------
    // File operations
    // --------------------------------------------------------
    ::Atlas::Bridge::BridgeResult openFile(
        const std::string&                          sessionToken,
        const ::Atlas::Bridge::OpenFileRequest&   request);

    // --------------------------------------------------------
    // Tool actions endpoint  POST /editor/tools/run  (Task 4.4)
    // --------------------------------------------------------
    ::Atlas::Bridge::ToolActionResult runToolAction(
        const std::string&                              sessionToken,
        const ::Atlas::Bridge::ToolActionRequest&     request);

    // --------------------------------------------------------
    // Epic 10 / Task 10.1 — Search roots  GET /project/search-roots
    // --------------------------------------------------------
    ::Atlas::Bridge::ProjectSearchRoots getSearchRoots(
        const std::string& sessionToken) const;

    // --------------------------------------------------------
    // Epic 10 / Task 10.2 — Builder / PCG tool hooks  POST /editor/tools/builder
    // --------------------------------------------------------
    ::Atlas::Bridge::BuilderToolResult runBuilderTool(
        const std::string&                           sessionToken,
        const ::Atlas::Bridge::BuilderToolRequest& request);

    // --------------------------------------------------------
    // Epic 10 / Task 10.3 — Richer editor state  GET /editor/state
    // --------------------------------------------------------
    ::Atlas::Bridge::EditorStateSnapshot getEditorState(
        const std::string& sessionToken) const;

    // --------------------------------------------------------
    // Epic 10 / Task 10.4 — Codegen workflow
    //   POST /codegen/propose
    //   GET  /codegen/diff
    //   POST /codegen/approve
    //   POST /codegen/apply
    // --------------------------------------------------------
    ::Atlas::Bridge::CodegenProposal proposeCodegen(
        const std::string&                              sessionToken,
        const ::Atlas::Bridge::CodegenProposalRequest& request);

    ::Atlas::Bridge::CodegenDiff getCodegenDiff(
        const std::string& sessionToken,
        const std::string& proposalId) const;

    ::Atlas::Bridge::CodegenApplyResult approveAndApplyCodegen(
        const std::string&                               sessionToken,
        const ::Atlas::Bridge::CodegenApprovalRequest& request);

    // --------------------------------------------------------
    // Epic 10 / Task 10.5 — Workspace dashboard  GET /workspace/dashboard
    // --------------------------------------------------------
    ::Atlas::Bridge::WorkspaceDashboard getWorkspaceDashboard(
        const std::string& sessionToken) const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    bool validateSession(const std::string& token) const;
    bool validateWriteSession(const std::string& token) const;
    bool isToolActionAllowed(::Atlas::Bridge::ToolActionId actionId) const;
    bool isBuilderToolActionAllowed(::Atlas::Bridge::BuilderToolActionId actionId) const;

    void log(const std::string& message);
    void auditLog(
        const std::string& requestId,
        const std::string& sessionId,
        const std::string& service,
        const std::string& operation,
        bool               success,
        const std::string& summary,
        bool               wasDryRun  = false,
        const std::string& failReason = {}) const;

    BridgeLogCallback   m_logCallback;
    BridgeAuditLogger*  m_auditLogger = nullptr;
    BridgeServiceConfig m_config;
    bool                m_running = false;
};

} // namespace NovaForge::Integration::AtlasAI

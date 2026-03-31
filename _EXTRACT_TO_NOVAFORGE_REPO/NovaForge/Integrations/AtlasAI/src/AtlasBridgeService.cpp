// AtlasBridgeService.cpp
// NovaForge-side AtlasAI bridge service implementation.

#include "AtlasBridgeService.h"
#include "BridgeSessionManager.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <atomic>
#include <mutex>

namespace NovaForge::Integration::AtlasAI
{

// ============================================================
// Internal implementation state
// ============================================================

struct AtlasBridgeService::Impl
{
    BridgeSessionManager sessionManager;

    // Allowed tool action IDs (original whitelist)
    std::unordered_set<uint32_t> allowedActionIds =
    {
        static_cast<uint32_t>(::Atlas::Bridge::ToolActionId::ValidateData),
        static_cast<uint32_t>(::Atlas::Bridge::ToolActionId::RunPCGPreview),
        static_cast<uint32_t>(::Atlas::Bridge::ToolActionId::OpenScene),
        static_cast<uint32_t>(::Atlas::Bridge::ToolActionId::FocusEntity),
        static_cast<uint32_t>(::Atlas::Bridge::ToolActionId::RegenerateSchemas),
    };

    // Allowed builder tool action IDs (Epic 10 / Task 10.2)
    std::unordered_set<uint32_t> allowedBuilderActionIds =
    {
        static_cast<uint32_t>(::Atlas::Bridge::BuilderToolActionId::ValidateData),
        static_cast<uint32_t>(::Atlas::Bridge::BuilderToolActionId::RunPCGPreview),
        static_cast<uint32_t>(::Atlas::Bridge::BuilderToolActionId::OpenScene),
        static_cast<uint32_t>(::Atlas::Bridge::BuilderToolActionId::FocusEntity),
        static_cast<uint32_t>(::Atlas::Bridge::BuilderToolActionId::RegenerateSchemas),
        static_cast<uint32_t>(::Atlas::Bridge::BuilderToolActionId::RunBuilderInspect),
        static_cast<uint32_t>(::Atlas::Bridge::BuilderToolActionId::RunPCGDiagnostics),
        static_cast<uint32_t>(::Atlas::Bridge::BuilderToolActionId::GeneratePCGPreview),
        static_cast<uint32_t>(::Atlas::Bridge::BuilderToolActionId::ValidateBuilderData),
    };

    std::atomic<uint32_t> buildIdCounter{0};
    std::atomic<uint32_t> proposalIdCounter{0};

    // Dashboard tracking — updated on each build/tool call
    mutable std::mutex dashboardMutex;
    std::string  lastBuildTarget;
    std::string  lastBuildId;
    bool         lastBuildSucceeded     = false;
    std::string  lastBuildTimestampUtc;
    std::string  lastToolAction;
    std::string  lastToolTimestampUtc;

    // Pending codegen proposals (proposalId → request snapshot)
    std::unordered_map<std::string, ::Atlas::Bridge::CodegenProposalRequest> pendingProposals;
};

// ============================================================
// Lifecycle
// ============================================================

AtlasBridgeService::AtlasBridgeService()
    : m_impl(std::make_unique<Impl>())
{
}

AtlasBridgeService::~AtlasBridgeService()
{
    stop();
}

bool AtlasBridgeService::start(const BridgeServiceConfig& config)
{
    if (m_running)
    {
        log("AtlasBridgeService: already running");
        return false;
    }

    m_config  = config;
    m_running = true;

    log("AtlasBridgeService: started — REST port "
        + std::to_string(config.restPort)
        + ", WS port "
        + std::to_string(config.wsPort)
        + (config.bindLoopbackOnly ? " [loopback-only]" : " [WARNING: open binding]"));

    return true;
}

void AtlasBridgeService::stop()
{
    if (!m_running)
        return;

    m_impl->sessionManager.revokeAll();
    m_running = false;
    log("AtlasBridgeService: stopped — all sessions revoked");
}

bool AtlasBridgeService::isRunning() const
{
    return m_running;
}

// ============================================================
// Logging and audit
// ============================================================

void AtlasBridgeService::setLogCallback(BridgeLogCallback callback)
{
    m_logCallback = std::move(callback);
}

void AtlasBridgeService::setAuditLogger(BridgeAuditLogger* logger)
{
    m_auditLogger = logger;
}

void AtlasBridgeService::log(const std::string& message)
{
    if (m_logCallback)
        m_logCallback(message);
}

void AtlasBridgeService::auditLog(
    const std::string& requestId,
    const std::string& sessionId,
    const std::string& service,
    const std::string& operation,
    bool               success,
    const std::string& summary,
    bool               wasDryRun,
    const std::string& failReason) const
{
    if (m_auditLogger)
    {
        m_auditLogger->log(
            requestId, sessionId, service, operation,
            success, summary, wasDryRun, failReason);
    }
}

// ============================================================
// Session management  (Task 4.1 prerequisite)
// ============================================================

::Atlas::Bridge::SessionConnectResponse AtlasBridgeService::connectSession(
    const ::Atlas::Bridge::SessionConnectRequest& request)
{
    if (!m_running)
    {
        ::Atlas::Bridge::SessionConnectResponse response;
        response.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::BackendUnavailable;
        response.result.message   = "Bridge service is not running";
        return response;
    }

    auto response = m_impl->sessionManager.createSession(
        request, ::Atlas::Bridge::kProtocolVersion);

    if (response.result.succeeded())
    {
        log("AtlasBridgeService: session connected — project=" + request.projectId
            + " token=" + response.sessionToken.substr(0, 8) + "...");
        auditLog({}, {}, "SessionService", "Connect",
                 true, "Session created for project=" + request.projectId);
    }
    else
    {
        log("AtlasBridgeService: session connect failed — " + response.result.message);
        auditLog({}, {}, "SessionService", "Connect",
                 false, "Session create failed",
                 false, response.result.message);
    }

    return response;
}

::Atlas::Bridge::BridgeResult AtlasBridgeService::disconnectSession(
    const std::string& sessionToken)
{
    ::Atlas::Bridge::BridgeResult result;

    if (!m_impl->sessionManager.validateToken(sessionToken))
    {
        result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        result.message   = "Invalid or expired session token";
        return result;
    }

    m_impl->sessionManager.revokeSession(sessionToken);
    log("AtlasBridgeService: session disconnected");
    auditLog({}, sessionToken, "SessionService", "Disconnect",
             true, "Session revoked");

    result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
    result.message   = "Session disconnected";
    return result;
}

// ============================================================
// Session validation helpers
// ============================================================

bool AtlasBridgeService::validateSession(const std::string& token) const
{
    return m_impl->sessionManager.validateToken(token);
}

bool AtlasBridgeService::validateWriteSession(const std::string& token) const
{
    return m_impl->sessionManager.isWriteAuthorized(token);
}

// ============================================================
// Project info  GET /project/info  (Task 4.1)
// ============================================================

::Atlas::Bridge::ProjectInfo AtlasBridgeService::getProjectInfo(
    const std::string& sessionToken) const
{
    ::Atlas::Bridge::ProjectInfo info;

    if (!validateSession(sessionToken))
    {
        // Return a minimal error indicator via the projectId field;
        // callers should check validateSession before calling this.
        info.projectId = "__unauthorized__";
        return info;
    }

    info.projectId   = "novaforge";
    info.displayName = "NovaForge";
    info.version     = "0.1.0";
    info.repoRoot    = ""; // populated at runtime from manifest path

    info.capabilities.supportsViewportAttach   = false;
    info.capabilities.supportsLivePatch        = false;
    info.capabilities.supportsAISession        = true;
    info.capabilities.supportsProjectIndexing  = true;
    info.capabilities.supportsMultiWorkspace   = false;

    return info;
}

// ============================================================
// Build  POST /build/run  (Task 4.3)
// ============================================================

::Atlas::Bridge::BuildResult AtlasBridgeService::runBuild(
    const std::string&                    sessionToken,
    const ::Atlas::Bridge::BuildTarget& target)
{
    ::Atlas::Bridge::BuildResult result;

    if (!m_running)
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::BackendUnavailable;
        result.result.message   = "Bridge service is not running";
        auditLog({}, sessionToken, "BuildService", "RunBuild",
                 false, "Service not running", false, result.result.message);
        return result;
    }

    if (!validateSession(sessionToken))
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        result.result.message   = "Invalid or expired session token";
        auditLog({}, sessionToken, "BuildService", "RunBuild",
                 false, "Unauthorized", false, result.result.message);
        return result;
    }

    const uint32_t buildId = ++m_impl->buildIdCounter;
    result.buildId = "build-" + std::to_string(buildId);

    log("AtlasBridgeService: build requested — target=" + target.name
        + " buildId=" + result.buildId);

    // TODO: integrate with actual build system (CMake/MSBuild dispatch)
    result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
    result.result.message   = "Build queued";
    result.exitCode         = 0;
    result.outputLog        = "Build stub: target '" + target.name + "' queued with id "
                              + result.buildId;

    // Update dashboard tracking
    {
        std::lock_guard<std::mutex> lk(m_impl->dashboardMutex);
        m_impl->lastBuildTarget       = target.name;
        m_impl->lastBuildId           = result.buildId;
        m_impl->lastBuildSucceeded    = true;
        m_impl->lastBuildTimestampUtc = m_impl->sessionManager.utcNowIso8601();
    }

    auditLog({}, sessionToken, "BuildService", "RunBuild",
             true, "Queued target=" + target.name + " id=" + result.buildId);

    return result;
}

// ============================================================
// Editor selection  GET /editor/selection  (Task 4.2)
// ============================================================

::Atlas::Bridge::EditorSelectionSnapshot AtlasBridgeService::getEditorSelection(
    const std::string& sessionToken) const
{
    ::Atlas::Bridge::EditorSelectionSnapshot snapshot;

    if (!validateSession(sessionToken))
        return snapshot; // empty snapshot — caller checks session separately

    // TODO: integrate with Atlas editor backend to retrieve live selection
    snapshot.activeScene        = "";
    snapshot.selectedObjectName = "";
    snapshot.selectedObjectType = "";
    snapshot.selectedObjectId   = 0;

    return snapshot;
}

// ============================================================
// File operations
// ============================================================

::Atlas::Bridge::BridgeResult AtlasBridgeService::openFile(
    const std::string&                        sessionToken,
    const ::Atlas::Bridge::OpenFileRequest& request)
{
    ::Atlas::Bridge::BridgeResult result;

    if (!m_running)
    {
        result.errorCode = ::Atlas::Bridge::BridgeErrorCode::BackendUnavailable;
        result.message   = "Bridge service is not running";
        return result;
    }

    if (!validateSession(sessionToken))
    {
        result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        result.message   = "Invalid or expired session token";
        auditLog({}, sessionToken, "EditorService", "OpenFile",
                 false, "Unauthorized", false, result.message);
        return result;
    }

    log("AtlasBridgeService: open file — " + request.filePath);

    // TODO: integrate with editor open-file command
    result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
    result.message   = "Open file stub: not yet implemented";

    auditLog({}, sessionToken, "EditorService", "OpenFile",
             true, "Requested file=" + request.filePath);

    return result;
}

// ============================================================
// Tool actions  POST /editor/tools/run  (Task 4.4)
// ============================================================

bool AtlasBridgeService::isToolActionAllowed(
    ::Atlas::Bridge::ToolActionId actionId) const
{
    return m_impl->allowedActionIds.count(
        static_cast<uint32_t>(actionId)) > 0;
}

::Atlas::Bridge::ToolActionResult AtlasBridgeService::runToolAction(
    const std::string&                              sessionToken,
    const ::Atlas::Bridge::ToolActionRequest&     request)
{
    ::Atlas::Bridge::ToolActionResult result;
    result.wasDryRun = request.dryRun;

    if (!m_running)
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::BackendUnavailable;
        result.result.message   = "Bridge service is not running";
        auditLog({}, sessionToken, "ToolService", "RunToolAction",
                 false, "Service not running", request.dryRun, result.result.message);
        return result;
    }

    if (!validateSession(sessionToken))
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        result.result.message   = "Invalid or expired session token";
        auditLog({}, sessionToken, "ToolService", "RunToolAction",
                 false, "Unauthorized", request.dryRun, result.result.message);
        return result;
    }

    // Require write authorization for non-dry-run actions
    if (!request.dryRun && !validateWriteSession(sessionToken))
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        result.result.message   = "Write authorization required for non-dry-run actions";
        auditLog({}, sessionToken, "ToolService", "RunToolAction",
                 false, "Write auth required", false, result.result.message);
        return result;
    }

    if (!isToolActionAllowed(request.actionId))
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::UnsupportedOp;
        result.result.message   = "Tool action is not in the whitelist";
        auditLog({}, sessionToken, "ToolService", "RunToolAction",
                 false,
                 "Denied action id=" + std::to_string(
                     static_cast<uint32_t>(request.actionId)),
                 request.dryRun,
                 result.result.message);
        return result;
    }

    const std::string actionIdStr =
        std::to_string(static_cast<uint32_t>(request.actionId));

    // Derive a human-readable name for dashboard tracking
    static const char* kActionNames[] = {
        "ValidateData", "RunPCGPreview", "OpenScene", "FocusEntity", "RegenerateSchemas"
    };
    const std::string actionName =
        (static_cast<uint32_t>(request.actionId) < 5u)
            ? kActionNames[static_cast<uint32_t>(request.actionId)]
            : actionIdStr;

    log("AtlasBridgeService: tool action id=" + actionIdStr
        + (request.dryRun ? " [DRY RUN]" : " [EXECUTE]"));

    // Update dashboard tracking
    {
        std::lock_guard<std::mutex> lk(m_impl->dashboardMutex);
        m_impl->lastToolAction       = actionName;
        m_impl->lastToolTimestampUtc = m_impl->sessionManager.utcNowIso8601();
    }

    if (request.dryRun)
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
        result.result.message   = "Dry-run: action would be permitted";
        result.summary          = "dry-run completed";
        auditLog({}, sessionToken, "ToolService", "RunToolAction",
                 true, "Dry-run id=" + actionIdStr, true);
        return result;
    }

    // TODO: route to actual editor tool implementations
    result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
    result.result.message   = "Tool action stub: not yet implemented";
    result.summary          = "stub executed";

    auditLog({}, sessionToken, "ToolService", "RunToolAction",
             true, "Executed id=" + actionIdStr, false);

    return result;
}

// ============================================================
// Epic 10 / Task 10.2 — Builder / PCG tool validation helper
// ============================================================

bool AtlasBridgeService::isBuilderToolActionAllowed(
    ::Atlas::Bridge::BuilderToolActionId actionId) const
{
    return m_impl->allowedBuilderActionIds.count(
        static_cast<uint32_t>(actionId)) > 0;
}

// ============================================================
// Epic 10 / Task 10.1 — Search roots  GET /project/search-roots
// ============================================================

::Atlas::Bridge::ProjectSearchRoots AtlasBridgeService::getSearchRoots(
    const std::string& sessionToken) const
{
    ::Atlas::Bridge::ProjectSearchRoots roots;

    if (!m_running)
    {
        roots.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::BackendUnavailable;
        roots.result.message   = "Bridge service is not running";
        return roots;
    }

    if (!validateSession(sessionToken))
    {
        roots.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        roots.result.message   = "Invalid or expired session token";
        return roots;
    }

    // Populate the standard NovaForge search roots (from project manifest layout)
    roots.roots =
    {
        { "Docs",              "Docs",             "docs"    },
        { "DataTables",        "NovaForge/Data",   "data"    },
        { "Content",           "NovaForge/Content","content" },
        { "Config",            "NovaForge/Data/Config", "config" },
        { "SourceAtlas",       "Atlas",            "source"  },
        { "SourceNovaForge",   "NovaForge",        "source"  },
        { "SourceAtlasAI",     "AtlasAI",          "source"  },
        { "SharedContracts",   "Shared",           "source"  },
    };

    roots.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
    roots.result.message   = "OK";

    auditLog({}, sessionToken, "ProjectService", "GetSearchRoots",
             true, "Returned " + std::to_string(roots.roots.size()) + " search roots");

    return roots;
}

// ============================================================
// Epic 10 / Task 10.2 — Builder / PCG tool hooks
//   POST /editor/tools/builder
// ============================================================

::Atlas::Bridge::BuilderToolResult AtlasBridgeService::runBuilderTool(
    const std::string&                           sessionToken,
    const ::Atlas::Bridge::BuilderToolRequest& request)
{
    ::Atlas::Bridge::BuilderToolResult result;
    result.wasDryRun = request.dryRun;

    if (!m_running)
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::BackendUnavailable;
        result.result.message   = "Bridge service is not running";
        return result;
    }

    if (!validateSession(sessionToken))
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        result.result.message   = "Invalid or expired session token";
        return result;
    }

    if (!request.dryRun && !validateWriteSession(sessionToken))
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        result.result.message   = "Write authorization required for non-dry-run builder actions";
        return result;
    }

    if (!isBuilderToolActionAllowed(request.actionId))
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::UnsupportedOp;
        result.result.message   = "Builder tool action is not in the whitelist";
        auditLog({}, sessionToken, "ToolService", "RunBuilderTool",
                 false,
                 "Denied builder action id="
                     + std::to_string(static_cast<uint32_t>(request.actionId)),
                 request.dryRun,
                 result.result.message);
        return result;
    }

    const std::string actionIdStr =
        std::to_string(static_cast<uint32_t>(request.actionId));

    log("AtlasBridgeService: builder tool action id=" + actionIdStr
        + (request.dryRun ? " [DRY RUN]" : " [EXECUTE]"));

    // Update dashboard tracking
    {
        std::lock_guard<std::mutex> lk(m_impl->dashboardMutex);
        m_impl->lastToolAction       = "Builder:" + actionIdStr;
        m_impl->lastToolTimestampUtc = m_impl->sessionManager.utcNowIso8601();
    }

    if (request.dryRun)
    {
        result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
        result.result.message   = "Dry-run: builder action would be permitted";
        result.summary          = "dry-run completed";
        result.diagnosticsLog   = "";
        auditLog({}, sessionToken, "ToolService", "RunBuilderTool",
                 true, "Dry-run builder id=" + actionIdStr, true);
        return result;
    }

    // Diagnostic actions populate the log field
    const bool isDiagnostic =
        (request.actionId == ::Atlas::Bridge::BuilderToolActionId::RunPCGDiagnostics
      || request.actionId == ::Atlas::Bridge::BuilderToolActionId::RunBuilderInspect);

    result.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
    result.result.message   = "Builder tool stub: not yet implemented";
    result.summary          = "stub executed";
    result.diagnosticsLog   = isDiagnostic
        ? "[stub] diagnostics output for action " + actionIdStr
        : "";

    auditLog({}, sessionToken, "ToolService", "RunBuilderTool",
             true, "Executed builder id=" + actionIdStr, false);

    return result;
}

// ============================================================
// Epic 10 / Task 10.3 — Richer editor state  GET /editor/state
// ============================================================

::Atlas::Bridge::EditorStateSnapshot AtlasBridgeService::getEditorState(
    const std::string& sessionToken) const
{
    ::Atlas::Bridge::EditorStateSnapshot snap;

    if (!m_running)
    {
        snap.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::BackendUnavailable;
        snap.result.message   = "Bridge service is not running";
        return snap;
    }

    if (!validateSession(sessionToken))
    {
        snap.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        snap.result.message   = "Invalid or expired session token";
        return snap;
    }

    // TODO: integrate with Atlas editor runtime to retrieve live state
    snap.result.errorCode       = ::Atlas::Bridge::BridgeErrorCode::Success;
    snap.activeScene            = "";  // populated by Atlas editor integration
    snap.activeMap              = "";
    snap.loadedWorldId          = "";
    snap.activeMode             = "Inspect"; // default editor mode
    snap.simulationState        = ::Atlas::Bridge::SimulationState::Stopped;
    snap.selectedObjectName     = "";
    snap.selectedObjectType     = "";
    snap.selectedObjectId       = 0;
    snap.selectedComponents     = {};

    auditLog({}, sessionToken, "EditorService", "GetEditorState",
             true, "Editor state snapshot returned");

    return snap;
}

// ============================================================
// Epic 10 / Task 10.4 — Codegen workflow
// ============================================================

::Atlas::Bridge::CodegenProposal AtlasBridgeService::proposeCodegen(
    const std::string&                               sessionToken,
    const ::Atlas::Bridge::CodegenProposalRequest& request)
{
    ::Atlas::Bridge::CodegenProposal proposal;

    if (!m_running)
    {
        proposal.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::BackendUnavailable;
        proposal.result.message   = "Bridge service is not running";
        return proposal;
    }

    if (!validateSession(sessionToken))
    {
        proposal.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        proposal.result.message   = "Invalid or expired session token";
        return proposal;
    }

    if (request.targetFile.empty())
    {
        proposal.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::InvalidRequest;
        proposal.result.message   = "targetFile must not be empty";
        return proposal;
    }

    const uint32_t pid = ++m_impl->proposalIdCounter;
    proposal.proposalId  = "proposal-" + std::to_string(pid);
    proposal.description = request.description;
    proposal.targetFile  = request.targetFile;
    proposal.summary     = "[stub] Proposal to change " + request.targetFile
                           + ": " + request.description;

    // Store for diff and approval look-up
    {
        std::lock_guard<std::mutex> lk(m_impl->dashboardMutex);
        m_impl->pendingProposals[proposal.proposalId] = request;
    }

    proposal.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
    proposal.result.message   = "Proposal created";

    log("AtlasBridgeService: codegen proposed — id=" + proposal.proposalId
        + " file=" + request.targetFile);

    auditLog({}, sessionToken, "CodegenService", "ProposeCodegen",
             true, "proposal=" + proposal.proposalId + " file=" + request.targetFile,
             request.dryRun);

    return proposal;
}

::Atlas::Bridge::CodegenDiff AtlasBridgeService::getCodegenDiff(
    const std::string& sessionToken,
    const std::string& proposalId) const
{
    ::Atlas::Bridge::CodegenDiff diff;

    if (!m_running)
    {
        diff.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::BackendUnavailable;
        diff.result.message   = "Bridge service is not running";
        return diff;
    }

    if (!validateSession(sessionToken))
    {
        diff.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        diff.result.message   = "Invalid or expired session token";
        return diff;
    }

    std::lock_guard<std::mutex> lk(m_impl->dashboardMutex);
    auto it = m_impl->pendingProposals.find(proposalId);
    if (it == m_impl->pendingProposals.end())
    {
        diff.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::NotFound;
        diff.result.message   = "Proposal not found: " + proposalId;
        return diff;
    }

    const auto& req = it->second;

    // Stub diff — in production this would be populated by the AI codegen engine
    diff.proposalId  = proposalId;
    diff.diffText    = "--- a/" + req.targetFile + "\n"
                       "+++ b/" + req.targetFile + "\n"
                       "@@ -0,0 +1,3 @@\n"
                       "+// Generated stub\n"
                       "+// " + req.description + "\n"
                       "+\n";
    diff.linesAdded   = 3;
    diff.linesRemoved = 0;

    diff.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
    diff.result.message   = "Diff generated";

    return diff;
}

::Atlas::Bridge::CodegenApplyResult AtlasBridgeService::approveAndApplyCodegen(
    const std::string&                               sessionToken,
    const ::Atlas::Bridge::CodegenApprovalRequest& request)
{
    ::Atlas::Bridge::CodegenApplyResult applyResult;

    if (!m_running)
    {
        applyResult.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::BackendUnavailable;
        applyResult.result.message   = "Bridge service is not running";
        return applyResult;
    }

    if (!validateSession(sessionToken))
    {
        applyResult.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        applyResult.result.message   = "Invalid or expired session token";
        return applyResult;
    }

    // Write authorization is only required when actually applying (approved=true);
    // rejection is read-only — it just discards the pending proposal.
    if (request.approved && !validateWriteSession(sessionToken))
    {
        applyResult.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        applyResult.result.message   = "Write authorization required to apply codegen";
        return applyResult;
    }

    std::lock_guard<std::mutex> lk(m_impl->dashboardMutex);
    auto it = m_impl->pendingProposals.find(request.proposalId);
    if (it == m_impl->pendingProposals.end())
    {
        applyResult.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::NotFound;
        applyResult.result.message   = "Proposal not found: " + request.proposalId;
        return applyResult;
    }

    applyResult.proposalId = request.proposalId;
    applyResult.wasApplied = request.approved;

    if (request.approved)
    {
        applyResult.appliedFile      = it->second.targetFile;
        applyResult.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
        applyResult.result.message   = "Proposal applied";

        log("AtlasBridgeService: codegen applied — proposal=" + request.proposalId
            + " file=" + applyResult.appliedFile);
        auditLog({}, sessionToken, "CodegenService", "ApplyCodegen",
                 true, "applied proposal=" + request.proposalId
                       + " file=" + applyResult.appliedFile);
    }
    else
    {
        applyResult.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
        applyResult.result.message   = "Proposal rejected — no changes made";
        auditLog({}, sessionToken, "CodegenService", "ApplyCodegen",
                 true, "rejected proposal=" + request.proposalId);
    }

    // Remove from pending regardless of outcome
    m_impl->pendingProposals.erase(it);

    return applyResult;
}

// ============================================================
// Epic 10 / Task 10.5 — Workspace dashboard  GET /workspace/dashboard
// ============================================================

::Atlas::Bridge::WorkspaceDashboard AtlasBridgeService::getWorkspaceDashboard(
    const std::string& sessionToken) const
{
    ::Atlas::Bridge::WorkspaceDashboard dash;

    if (!m_running)
    {
        dash.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::BackendUnavailable;
        dash.result.message   = "Bridge service is not running";
        return dash;
    }

    if (!validateSession(sessionToken))
    {
        dash.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Unauthorized;
        dash.result.message   = "Invalid or expired session token";
        return dash;
    }

    dash.result.errorCode  = ::Atlas::Bridge::BridgeErrorCode::Success;
    dash.projectId         = "novaforge";
    dash.projectName       = "NovaForge";
    dash.projectVersion    = "0.1.0";
    dash.activeSessionCount =
        m_impl->sessionManager.activeSessionCount();

    // Build health snapshot
    {
        std::lock_guard<std::mutex> lk(m_impl->dashboardMutex);
        dash.buildHealth.lastBuildTarget       = m_impl->lastBuildTarget;
        dash.buildHealth.lastBuildId           = m_impl->lastBuildId;
        dash.buildHealth.lastBuildSucceeded    = m_impl->lastBuildSucceeded;
        dash.buildHealth.lastBuildTimestampUtc = m_impl->lastBuildTimestampUtc;
        dash.lastToolAction                    = m_impl->lastToolAction;
        dash.lastToolTimestampUtc              = m_impl->lastToolTimestampUtc;
    }

    // Search roots — reuse the getSearchRoots logic
    {
        // Temporarily unlock to avoid deadlock; validate token already done above
        dash.searchRoots.roots =
        {
            { "Docs",            "Docs",              "docs"    },
            { "DataTables",      "NovaForge/Data",    "data"    },
            { "Content",         "NovaForge/Content", "content" },
            { "Config",          "NovaForge/Data/Config", "config" },
            { "SourceAtlas",     "Atlas",             "source"  },
            { "SourceNovaForge", "NovaForge",         "source"  },
            { "SourceAtlasAI",   "AtlasAI",           "source"  },
            { "SharedContracts", "Shared",            "source"  },
        };
        dash.searchRoots.result.errorCode = ::Atlas::Bridge::BridgeErrorCode::Success;
    }

    dash.currentProjectStatus = m_impl->lastBuildTarget.empty() ? "idle" : "idle";

    auditLog({}, sessionToken, "WorkspaceService", "GetDashboard",
             true, "Dashboard returned");

    return dash;
}

} // namespace NovaForge::Integration::AtlasAI

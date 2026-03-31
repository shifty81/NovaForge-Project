// NovaForgeProjectAdapter.cs
// AtlasAI project adapter for NovaForge.
//
// This adapter:
// - loads the NovaForge project manifest
// - connects to the NovaForge AtlasAI bridge service via /session/connect
// - exposes project info, build targets, and whitelisted tool actions to AtlasAI
// - wraps all requests in the standard BridgeRequestEnvelope
//
// Epic 6 / Task 6.2 — project-specific adapter implements IProjectAdapter
//
// Rules:
// - must not directly access gameplay runtime internals
// - must not contain WPF or UI code
// - all operations go through the bridge protocol defined in Shared/ToolProtocol

using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;

// Pull in IProjectAdapter and BridgeResponse from the parent namespace
using AtlasAI.ProjectAdapters;

namespace AtlasAI.ProjectAdapters.NovaForge
{
    public sealed class NovaForgeProjectAdapter : IProjectAdapter
    {
        private readonly NovaForgeProjectManifest _manifest;
        private readonly HttpClient               _http;
        private string                            _sessionToken = string.Empty;
        private bool                              _disposed;

        private static readonly JsonSerializerOptions s_jsonOptions = new()
        {
            PropertyNameCaseInsensitive = true,
        };

        public string ProjectId      => _manifest.Project.Id;
        public string ProjectName    => _manifest.Project.DisplayName;
        public string ProjectVersion => _manifest.Project.Version;

        /// <summary>Returns the current session token, or empty if not connected.</summary>
        public string SessionToken => _sessionToken;

        public NovaForgeProjectAdapter(string repoRoot)
        {
            string manifestPath = NovaForgeProjectManifest.ResolveManifestPath(repoRoot);
            _manifest = NovaForgeProjectManifest.Load(manifestPath);

            _http = new HttpClient
            {
                BaseAddress = new Uri(
                    $"http://{_manifest.Bridge.Host}:{_manifest.Bridge.RestPort}/"),
                Timeout = TimeSpan.FromSeconds(_manifest.Bridge.TimeoutSeconds),
            };
        }

        // ----------------------------------------------------------------
        // Project info (local — from manifest, no network call)
        // ----------------------------------------------------------------

        /// <summary>Returns cached project info from the manifest.</summary>
        public ProjectInfoModel GetProjectInfo() => _manifest.Project;

        /// <summary>Returns capability flags from the manifest.</summary>
        public ProjectCapabilitiesModel GetCapabilities() => _manifest.Capabilities;

        /// <summary>Returns all configured build targets.</summary>
        public IReadOnlyList<BuildTargetModel> GetBuildTargets() =>
            _manifest.BuildTargets;

        /// <summary>
        /// IProjectAdapter: returns build target names for generic consumers.
        /// </summary>
        public IReadOnlyList<string> GetBuildTargetNames() =>
            _manifest.BuildTargets.Select(t => t.Name).ToList();

        /// <summary>Returns all allowed tool actions from safety settings.</summary>
        public IReadOnlyList<string> GetAllowedToolActions() =>
            _manifest.SafetySettings.AllowedToolActions;

        /// <summary>Returns the list of allowed builder tool action names.</summary>
        public IReadOnlyList<string> GetAllowedBuilderToolActions() =>
            new List<string>
            {
                "ValidateData", "RunPCGPreview", "OpenScene", "FocusEntity",
                "RegenerateSchemas",
                // Epic 10 / Task 10.2
                "RunBuilderInspect", "RunPCGDiagnostics",
                "GeneratePCGPreview", "ValidateBuilderData",
            };

        // ----------------------------------------------------------------
        // Session management  (Task 4.1 prerequisite — /session/connect)
        // ----------------------------------------------------------------

        /// <summary>
        /// Establishes a bridge session with the NovaForge backend.
        /// The session token is stored and used for all subsequent requests.
        /// </summary>
        public async Task<BridgeResponse> ConnectSessionAsync(
            CancellationToken cancellationToken = default)
        {
            var payload = new SessionConnectPayload
            {
                ProjectId = _manifest.Project.Id,
            };

            var envelope = BridgeRequestEnvelope.Create(
                service:   "SessionService",
                operation: "Connect",
                payload:   payload);

            var response = await PostEnvelopeAsync(
                "session/connect", envelope, cancellationToken);

            if (response.Success)
            {
                // Extract the session token from the response payload
                try
                {
                    var parsed = JsonSerializer.Deserialize<BridgeResponseEnvelope>(
                        response.Body, s_jsonOptions);

                    if (parsed?.Payload.HasValue == true)
                    {
                        var connect = JsonSerializer.Deserialize<SessionConnectResponse>(
                            parsed.Payload.Value.GetRawText(), s_jsonOptions);
                        if (connect != null)
                            _sessionToken = connect.SessionToken;
                    }

                    if (string.IsNullOrEmpty(_sessionToken))
                    {
                        // Server responded 200 but payload had no token — treat as failure
                        return new BridgeResponse(false,
                            "Session connect succeeded but no session token was returned.");
                    }
                }
                catch (JsonException ex)
                {
                    // Deserialization failed — surface the error to the caller
                    return new BridgeResponse(false,
                        $"Session connect succeeded but token extraction failed: {ex.Message}");
                }
            }

            return response;
        }

        /// <summary>Disconnects the current bridge session.</summary>
        public async Task<BridgeResponse> DisconnectSessionAsync(
            CancellationToken cancellationToken = default)
        {
            var response = await PostEnvelopeAsync(
                "session/disconnect",
                BuildEnvelope("SessionService", "Disconnect", null),
                cancellationToken);

            _sessionToken = string.Empty;
            return response;
        }

        // ----------------------------------------------------------------
        // Bridge connectivity
        // ----------------------------------------------------------------

        /// <summary>
        /// Checks whether the NovaForge bridge service is reachable.
        /// </summary>
        public async Task<bool> IsBackendReachableAsync(
            CancellationToken cancellationToken = default)
        {
            try
            {
                var response = await _http.GetAsync(
                    "project/info", cancellationToken);
                return response.IsSuccessStatusCode;
            }
            catch
            {
                return false;
            }
        }

        // ----------------------------------------------------------------
        // Build  POST /build/run  (Task 4.3)
        // ----------------------------------------------------------------

        /// <summary>
        /// Requests a build via the bridge service.
        /// Requires an active session (call ConnectSessionAsync first).
        /// </summary>
        public async Task<BridgeResponse> RequestBuildAsync(
            string              targetName,
            string              configuration    = "Debug",
            CancellationToken   cancellationToken = default)
        {
            var payload = new
            {
                target        = targetName,
                configuration = configuration,
                platform      = "Win64",
                rebuild       = false,
            };

            return await PostEnvelopeAsync(
                "build/run",
                BuildEnvelope("BuildService", "RunBuild", payload),
                cancellationToken);
        }

        // ----------------------------------------------------------------
        // Editor state  GET /editor/selection  (Task 4.2)
        // ----------------------------------------------------------------

        /// <summary>
        /// Queries the current editor selection snapshot.
        /// Requires an active session.
        /// </summary>
        public async Task<BridgeResponse> GetEditorSelectionAsync(
            CancellationToken cancellationToken = default)
        {
            try
            {
                var request = new HttpRequestMessage(
                    HttpMethod.Get, "editor/selection");
                AddSessionHeader(request);

                var response = await _http.SendAsync(request, cancellationToken);
                string content = await response.Content.ReadAsStringAsync(cancellationToken);
                return new BridgeResponse(response.IsSuccessStatusCode, content);
            }
            catch (Exception ex)
            {
                return new BridgeResponse(false, ex.Message);
            }
        }

        // ----------------------------------------------------------------
        // Tool actions  POST /editor/tools/run  (Task 4.4)
        // ----------------------------------------------------------------

        /// <summary>
        /// Runs a whitelisted tool action.
        /// Defaults to dry-run for safety. Requires an active session.
        /// </summary>
        public async Task<BridgeResponse> RunToolActionAsync(
            string              actionName,
            string?             parameter         = null,
            bool                dryRun            = true,
            CancellationToken   cancellationToken  = default)
        {
            if (!IsActionAllowed(actionName))
                return new BridgeResponse(false,
                    $"Tool action '{actionName}' is not in the allowed list.");

            var payload = new
            {
                action    = actionName,
                parameter = parameter ?? string.Empty,
                dryRun    = dryRun,
            };

            return await PostEnvelopeAsync(
                "editor/tools/run",
                BuildEnvelope("ToolService", "RunToolAction", payload),
                cancellationToken);
        }

        // ----------------------------------------------------------------
        // Path helpers
        // ----------------------------------------------------------------

        /// <summary>Returns the absolute path to the data root.</summary>
        public string GetDataRoot(string repoRoot) =>
            System.IO.Path.Combine(repoRoot, _manifest.RepoPaths.DataRoot);

        /// <summary>Returns the absolute path to the content root.</summary>
        public string GetContentRoot(string repoRoot) =>
            System.IO.Path.Combine(repoRoot, _manifest.RepoPaths.ContentRoot);

        /// <summary>Returns the absolute path to the docs root.</summary>
        public string GetDocsRoot(string repoRoot) =>
            System.IO.Path.Combine(repoRoot, _manifest.RepoPaths.DocsRoot);

        // ----------------------------------------------------------------
        // Epic 10 / Task 10.1 — Search roots  GET /project/search-roots
        // ----------------------------------------------------------------

        /// <summary>
        /// Returns the full set of searchable project roots
        /// (docs, data, content, config, source).
        /// </summary>
        public async Task<BridgeResponse> GetSearchRootsAsync(
            CancellationToken cancellationToken = default)
        {
            try
            {
                var request = new HttpRequestMessage(HttpMethod.Get, "project/search-roots");
                AddSessionHeader(request);
                var response = await _http.SendAsync(request, cancellationToken);
                string content = await response.Content.ReadAsStringAsync(cancellationToken);
                return new BridgeResponse(response.IsSuccessStatusCode, content);
            }
            catch (Exception ex)
            {
                return new BridgeResponse(false, ex.Message);
            }
        }

        // ----------------------------------------------------------------
        // Epic 10 / Task 10.2 — Builder / PCG tool hooks
        // ----------------------------------------------------------------

        /// <summary>
        /// Runs a whitelisted builder or PCG tool hook.
        /// Defaults to dry-run for safety.
        /// </summary>
        public async Task<BridgeResponse> RunBuilderToolAsync(
            string            actionName,
            string?           sceneTarget       = null,
            string?           parameter         = null,
            bool              dryRun            = true,
            CancellationToken cancellationToken  = default)
        {
            if (!IsBuilderActionAllowed(actionName))
                return new BridgeResponse(false,
                    $"Builder tool action '{actionName}' is not in the allowed list.");

            var payload = new
            {
                action      = actionName,
                sceneTarget = sceneTarget ?? string.Empty,
                parameter   = parameter   ?? string.Empty,
                dryRun      = dryRun,
            };

            return await PostEnvelopeAsync(
                "editor/tools/builder",
                BuildEnvelope("ToolService", "RunBuilderTool", payload),
                cancellationToken);
        }

        // ----------------------------------------------------------------
        // Epic 10 / Task 10.3 — Richer editor state  GET /editor/state
        // ----------------------------------------------------------------

        /// <summary>
        /// Returns the full editor state snapshot including active map, mode,
        /// world state, selected components, and simulation state.
        /// </summary>
        public async Task<BridgeResponse> GetEditorStateAsync(
            CancellationToken cancellationToken = default)
        {
            try
            {
                var request = new HttpRequestMessage(HttpMethod.Get, "editor/state");
                AddSessionHeader(request);
                var response = await _http.SendAsync(request, cancellationToken);
                string content = await response.Content.ReadAsStringAsync(cancellationToken);
                return new BridgeResponse(response.IsSuccessStatusCode, content);
            }
            catch (Exception ex)
            {
                return new BridgeResponse(false, ex.Message);
            }
        }

        // ----------------------------------------------------------------
        // Epic 10 / Task 10.4 — Codegen proposal workflow
        // ----------------------------------------------------------------

        /// <summary>
        /// Proposes a code-generation change for human review.
        /// Returns a proposal ID used in the diff / approve steps.
        /// Never applies changes directly.
        /// </summary>
        public async Task<BridgeResponse> ProposeCodegenAsync(
            string            description,
            string            targetFile,
            string?           context           = null,
            CancellationToken cancellationToken  = default)
        {
            var payload = new
            {
                description = description,
                targetFile  = targetFile,
                context     = context ?? string.Empty,
                dryRun      = true,
            };

            return await PostEnvelopeAsync(
                "codegen/propose",
                BuildEnvelope("CodegenService", "ProposeCodegen", payload),
                cancellationToken);
        }

        /// <summary>Gets the unified-diff preview for a pending codegen proposal.</summary>
        public async Task<BridgeResponse> GetCodegenDiffAsync(
            string            proposalId,
            CancellationToken cancellationToken = default)
        {
            try
            {
                var request = new HttpRequestMessage(
                    HttpMethod.Get, $"codegen/diff/{Uri.EscapeDataString(proposalId)}");
                AddSessionHeader(request);
                var response = await _http.SendAsync(request, cancellationToken);
                string content = await response.Content.ReadAsStringAsync(cancellationToken);
                return new BridgeResponse(response.IsSuccessStatusCode, content);
            }
            catch (Exception ex)
            {
                return new BridgeResponse(false, ex.Message);
            }
        }

        /// <summary>
        /// Approves or rejects a pending codegen proposal.
        /// Approval requires a write-authorized bridge session.
        /// Rejection simply discards the proposal; no write auth is required.
        /// </summary>
        public async Task<BridgeResponse> ApproveCodegenAsync(
            string            proposalId,
            bool              approved,
            string?           comment           = null,
            CancellationToken cancellationToken  = default)
        {
            var payload = new
            {
                proposalId = proposalId,
                approved   = approved,
                comment    = comment ?? string.Empty,
            };

            return await PostEnvelopeAsync(
                "codegen/approve",
                BuildEnvelope("CodegenService", "ApproveCodegen", payload),
                cancellationToken);
        }

        // ----------------------------------------------------------------
        // Epic 10 / Task 10.5 — Workspace dashboard  GET /workspace/dashboard
        // ----------------------------------------------------------------

        /// <summary>
        /// Returns the aggregated workspace dashboard from the backend bridge service.
        /// Includes build health, recent actions, search roots, and project status.
        /// </summary>
        public async Task<BridgeResponse> GetWorkspaceDashboardAsync(
            CancellationToken cancellationToken = default)
        {
            try
            {
                var request = new HttpRequestMessage(HttpMethod.Get, "workspace/dashboard");
                AddSessionHeader(request);
                var response = await _http.SendAsync(request, cancellationToken);
                string content = await response.Content.ReadAsStringAsync(cancellationToken);
                return new BridgeResponse(response.IsSuccessStatusCode, content);
            }
            catch (Exception ex)
            {
                return new BridgeResponse(false, ex.Message);
            }
        }

        // ----------------------------------------------------------------
        // Private helpers
        // ----------------------------------------------------------------

        private bool IsActionAllowed(string actionName)
        {
            foreach (var allowed in _manifest.SafetySettings.AllowedToolActions)
            {
                if (string.Equals(allowed, actionName,
                        StringComparison.OrdinalIgnoreCase))
                    return true;
            }
            return false;
        }

        private bool IsBuilderActionAllowed(string actionName)
        {
            foreach (var allowed in GetAllowedBuilderToolActions())
            {
                if (string.Equals(allowed, actionName,
                        StringComparison.OrdinalIgnoreCase))
                    return true;
            }
            return false;
        }

        private BridgeRequestEnvelope BuildEnvelope(
            string  service,
            string  operation,
            object? payload) =>
            BridgeRequestEnvelope.Create(
                service:   service,
                operation: operation,
                sessionId: _sessionToken,
                payload:   payload);

        private void AddSessionHeader(HttpRequestMessage request)
        {
            if (!string.IsNullOrEmpty(_sessionToken))
                request.Headers.TryAddWithoutValidation(
                    "X-Bridge-Session", _sessionToken);
        }

        private async Task<BridgeResponse> PostEnvelopeAsync(
            string                  endpoint,
            BridgeRequestEnvelope   envelope,
            CancellationToken       cancellationToken)
        {
            try
            {
                string json    = JsonSerializer.Serialize(envelope);
                var    content = new StringContent(json, Encoding.UTF8, "application/json");

                var httpRequest = new HttpRequestMessage(HttpMethod.Post, endpoint)
                {
                    Content = content,
                };
                AddSessionHeader(httpRequest);

                var    response = await _http.SendAsync(httpRequest, cancellationToken);
                string body     = await response.Content.ReadAsStringAsync(cancellationToken);
                return new BridgeResponse(response.IsSuccessStatusCode, body);
            }
            catch (Exception ex)
            {
                return new BridgeResponse(false, ex.Message);
            }
        }

        // ----------------------------------------------------------------
        // IDisposable
        // ----------------------------------------------------------------

        public void Dispose()
        {
            if (!_disposed)
            {
                _http.Dispose();
                _disposed = true;
            }
        }
    }

}

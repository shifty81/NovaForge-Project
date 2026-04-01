// BridgeRequestEnvelope.cs
// C# model for the standard JSON request envelope used in all bridge calls.
// Mirrors the ToolProtocol specification in Shared/ToolProtocol/README.md.

using System;
using System.Text.Json.Serialization;

namespace AtlasAI.ProjectAdapters.NovaForge
{
    /// <summary>
    /// Standard wrapper for every request sent from AtlasAI to the NovaForge bridge.
    /// Use <see cref="Create"/> to get a correctly initialised instance.
    /// </summary>
    public sealed class BridgeRequestEnvelope
    {
        [JsonPropertyName("protocolVersion")]
        public string ProtocolVersion { get; init; } = BridgeProtocol.Version;

        [JsonPropertyName("requestId")]
        public string RequestId { get; init; } = Guid.NewGuid().ToString();

        [JsonPropertyName("sessionId")]
        public string SessionId { get; init; } = string.Empty;

        [JsonPropertyName("service")]
        public string Service { get; init; } = string.Empty;

        [JsonPropertyName("operation")]
        public string Operation { get; init; } = string.Empty;

        [JsonPropertyName("timestampUtc")]
        public string TimestampUtc { get; init; } = DateTime.UtcNow.ToString("o");

        [JsonPropertyName("payload")]
        public object? Payload { get; init; }

        /// <summary>
        /// Factory method that ensures ProtocolVersion and TimestampUtc are
        /// always set at construction time rather than relying on property
        /// initialiser evaluation order.
        /// </summary>
        public static BridgeRequestEnvelope Create(
            string  service,
            string  operation,
            string  sessionId = "",
            object? payload   = null) =>
            new BridgeRequestEnvelope
            {
                ProtocolVersion = BridgeProtocol.Version,
                RequestId       = Guid.NewGuid().ToString(),
                SessionId       = sessionId,
                Service         = service,
                Operation       = operation,
                TimestampUtc    = DateTime.UtcNow.ToString("o"),
                Payload         = payload,
            };
    }

    /// <summary>
    /// Standard wrapper for every response received from the NovaForge bridge.
    /// </summary>
    public sealed class BridgeResponseEnvelope
    {
        [JsonPropertyName("protocolVersion")]
        public string ProtocolVersion { get; init; } = BridgeProtocol.Version;

        [JsonPropertyName("requestId")]
        public string RequestId { get; init; } = string.Empty;

        [JsonPropertyName("success")]
        public bool Success { get; init; }

        [JsonPropertyName("errorCode")]
        public string? ErrorCode { get; init; }

        [JsonPropertyName("message")]
        public string Message { get; init; } = string.Empty;

        [JsonPropertyName("payload")]
        public System.Text.Json.JsonElement? Payload { get; init; }
    }

    /// <summary>
    /// Session connect request payload.
    /// Sent to <c>/session/connect</c> to obtain a session token.
    /// </summary>
    public sealed class SessionConnectPayload
    {
        [JsonPropertyName("protocolVersion")]
        public string ProtocolVersion { get; init; } = BridgeProtocol.Version;

        [JsonPropertyName("clientVersion")]
        public string ClientVersion { get; init; } = BridgeProtocol.ClientVersion;

        [JsonPropertyName("projectId")]
        public string ProjectId { get; init; } = string.Empty;
    }

    /// <summary>
    /// Session connect response payload.
    /// </summary>
    public sealed class SessionConnectResponse
    {
        [JsonPropertyName("sessionToken")]
        public string SessionToken { get; init; } = string.Empty;

        [JsonPropertyName("serverVersion")]
        public string ServerVersion { get; init; } = string.Empty;

        [JsonPropertyName("projectId")]
        public string ProjectId { get; init; } = string.Empty;

        [JsonPropertyName("writeEnabled")]
        public bool WriteEnabled { get; init; }
    }

    /// <summary>
    /// Protocol version constants shared across all envelope types.
    /// </summary>
    public static class BridgeProtocol
    {
        public const string Version       = "1.0";
        public const string ClientVersion = "0.1.0";
    }
}

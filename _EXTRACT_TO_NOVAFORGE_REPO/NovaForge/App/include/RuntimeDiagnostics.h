#pragma once
// RuntimeDiagnostics.h
// NovaForge App — M6 runtime diagnostics reporter.

#include <string>
#include <vector>

namespace NovaForge::App
{

/// <summary>
/// Health record for a single engine subsystem.
/// Written to the diagnostics JSON report after a headless session.
/// </summary>
struct SubsystemHealth
{
    enum class Severity { Pass, Warn, Error };

    std::string name;
    Severity    severity = Severity::Pass;
    std::string message;

    /// Returns "pass", "warn", or "error".
    std::string SeverityString() const;
};

/// <summary>
/// Collects per-subsystem health data during a headless session and
/// serialises it to a JSON report file that Atlas Suite can parse.
/// </summary>
class RuntimeDiagnostics
{
public:
    explicit RuntimeDiagnostics(std::string sessionId);

    /// Records the health of a single subsystem.
    void RecordSubsystem(std::string name,
                         SubsystemHealth::Severity severity,
                         std::string message);

    /// Returns true if any subsystem reported an Error.
    bool HasErrors() const;

    /// Serialises the report to an in-memory JSON string.
    std::string ToJson() const;

    /// Writes the JSON report to <paramref name="outputPath"/>.
    bool WriteReport(const std::string& outputPath) const;

    // ── Factory ──────────────────────────────────────────────────────────

    /// Builds a <see cref="RuntimeDiagnostics"/> from a completed
    /// <c>PlaytestSession</c> result and adds stub subsystem health rows.
    static RuntimeDiagnostics FromPlaytestResult(
        const std::string&              sessionId,
        bool                            passed,
        int                             errorCount,
        const std::vector<std::string>& errors);

private:
    std::string                  m_sessionId;
    std::vector<SubsystemHealth> m_subsystems;
};

} // namespace NovaForge::App

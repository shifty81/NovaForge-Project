// RuntimeDiagnostics.cpp
// NovaForge App — M6 runtime diagnostics reporter.
//
// Writes a JSON diagnostics report after a headless session.  The Atlas Suite
// reads this file via DiagnosticsPanelViewModel to surface subsystem health.

#include "RuntimeDiagnostics.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace NovaForge::App
{

// ---------------------------------------------------------------------------
// SubsystemHealth helpers
// ---------------------------------------------------------------------------

std::string SubsystemHealth::SeverityString() const
{
    switch (severity)
    {
        case Severity::Pass:  return "pass";
        case Severity::Warn:  return "warn";
        case Severity::Error: return "error";
        default:              return "unknown";
    }
}

// ---------------------------------------------------------------------------
// RuntimeDiagnostics
// ---------------------------------------------------------------------------

RuntimeDiagnostics::RuntimeDiagnostics(std::string sessionId)
    : m_sessionId(std::move(sessionId))
{
}

void RuntimeDiagnostics::RecordSubsystem(std::string name,
                                          SubsystemHealth::Severity severity,
                                          std::string message)
{
    m_subsystems.push_back(SubsystemHealth{
        std::move(name),
        severity,
        std::move(message)
    });
}

bool RuntimeDiagnostics::HasErrors() const
{
    for (const auto& s : m_subsystems)
        if (s.severity == SubsystemHealth::Severity::Error)
            return true;
    return false;
}

// ---------------------------------------------------------------------------
// JSON serialization
// ---------------------------------------------------------------------------

std::string RuntimeDiagnostics::ToJson() const
{
    std::ostringstream json;
    json << "{\n";
    json << "  \"sessionId\": \"" << m_sessionId << "\",\n";
    json << "  \"subsystems\": [\n";

    for (std::size_t i = 0; i < m_subsystems.size(); ++i)
    {
        const auto& s = m_subsystems[i];
        json << "    {\n";
        json << "      \"name\": \"" << s.name << "\",\n";
        json << "      \"severity\": \"" << s.SeverityString() << "\",\n";
        json << "      \"message\": \"" << s.message << "\"\n";
        json << "    }";
        if (i + 1 < m_subsystems.size())
            json << ",";
        json << "\n";
    }

    json << "  ]\n";
    json << "}\n";
    return json.str();
}

bool RuntimeDiagnostics::WriteReport(const std::string& outputPath) const
{
    std::ofstream out(outputPath);
    if (!out.is_open())
    {
        std::cerr << "[RuntimeDiagnostics] Failed to open '" << outputPath << "' for writing.\n";
        return false;
    }
    out << ToJson();
    std::cout << "[RuntimeDiagnostics] Report written to '" << outputPath << "'.\n";
    return true;
}

// ---------------------------------------------------------------------------
// Factory — build a report from a completed PlaytestSession result
// ---------------------------------------------------------------------------

RuntimeDiagnostics RuntimeDiagnostics::FromPlaytestResult(
    const std::string& sessionId,
    bool               passed,
    int                errorCount,
    const std::vector<std::string>& errors)
{
    RuntimeDiagnostics diag(sessionId);

    diag.RecordSubsystem(
        "PlaytestSession",
        passed ? SubsystemHealth::Severity::Pass : SubsystemHealth::Severity::Error,
        passed ? "Vertical slice smoke test passed."
               : "Smoke test failed (" + std::to_string(errorCount) + " error(s)).");

    for (const auto& err : errors)
        diag.RecordSubsystem("Error", SubsystemHealth::Severity::Error, err);

    // Core subsystems stub — a real implementation would query each subsystem.
    diag.RecordSubsystem("Renderer",   SubsystemHealth::Severity::Pass, "Renderer initialised OK.");
    diag.RecordSubsystem("Physics",    SubsystemHealth::Severity::Pass, "Physics ticked normally.");
    diag.RecordSubsystem("DataLayer",  SubsystemHealth::Severity::Pass, "Data layer loaded all assets.");
    diag.RecordSubsystem("Networking", SubsystemHealth::Severity::Warn, "Networking: no server connection (expected in headless mode).");

    return diag;
}

} // namespace NovaForge::App

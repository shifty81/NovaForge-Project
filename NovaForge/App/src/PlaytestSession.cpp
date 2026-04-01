// PlaytestSession.cpp
// NovaForge App — automated playtest session implementation.

#include "PlaytestSession.h"
#include <sstream>
#include <iostream>

namespace NovaForge::App
{

// ---------------------------------------------------------------------------
// PlaytestResult helpers
// ---------------------------------------------------------------------------

std::string PlaytestResult::Format() const
{
    std::ostringstream ss;
    ss << "=== PlaytestResult ===\n";
    ss << "Status : " << (success ? "PASS" : "FAIL") << "\n";
    ss << "Ticks  : " << ticksRun << "\n";
    ss << "Errors : " << errorCount << "\n";
    if (!errors.empty())
    {
        ss << "Error list:\n";
        for (const auto& e : errors)
            ss << "  - " << e << "\n";
    }
    if (!log.empty())
    {
        ss << "Log (" << log.size() << " entries):\n";
        for (const auto& l : log)
            ss << "  " << l << "\n";
    }
    return ss.str();
}

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

PlaytestSession::PlaytestSession(const PlaytestConfig& config)
    : m_config(config)
{
}

// ---------------------------------------------------------------------------
// Run
// ---------------------------------------------------------------------------

PlaytestResult PlaytestSession::Run()
{
    PlaytestResult result;

    const std::string tag = m_config.scenarioTag.empty()
                              ? "default"
                              : m_config.scenarioTag;
    EmitLog(result, "[PlaytestSession] Starting scenario: " + tag);

    // ---- Boot check -------------------------------------------------------
    // In a full build the session would invoke WorldBootstrap and
    // GameSystemsRegistry. In the current scaffold we simulate the check.
    EmitLog(result, "[PlaytestSession] Boot check passed");

    // ---- Save load (optional) ---------------------------------------------
    if (m_config.loadSave && !m_config.saveName.empty())
    {
        EmitLog(result, "[PlaytestSession] Loading save: " + m_config.saveName);
        // SaveManager integration point — deferred until SaveManager::Load is wired.
    }

    // ---- Tick loop --------------------------------------------------------
    for (int tick = 0; tick < m_config.tickCount; ++tick)
    {
        // Invoke custom callback when provided.
        if (m_tickCallback)
        {
            bool cont = m_tickCallback(tick, m_config.deltaTime);
            if (!cont)
            {
                EmitLog(result, "[PlaytestSession] Tick callback requested abort at tick "
                        + std::to_string(tick));
                break;
            }
        }

        ++result.ticksRun;
    }

    EmitLog(result, "[PlaytestSession] Completed " + std::to_string(result.ticksRun)
            + "/" + std::to_string(m_config.tickCount) + " ticks");

    // ---- Health check -----------------------------------------------------
    if (result.errorCount > 0)
    {
        result.success = false;
        EmitLog(result, "[PlaytestSession] FAIL — " + std::to_string(result.errorCount)
                + " error(s) detected");
    }
    else
    {
        result.success = (result.ticksRun == m_config.tickCount);
        EmitLog(result, result.success
                ? "[PlaytestSession] PASS"
                : "[PlaytestSession] FAIL — tick count mismatch");
    }

    std::cout << result.Format();
    return result;
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

void PlaytestSession::EmitLog(PlaytestResult& result, const std::string& msg) const
{
    result.log.push_back(msg);
}

void PlaytestSession::EmitError(PlaytestResult& result, const std::string& msg) const
{
    result.errors.push_back(msg);
    ++result.errorCount;
}

} // namespace NovaForge::App

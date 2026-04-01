// PlaytestSession.h
// NovaForge App — automated playtest/smoke-test session.
//
// PlaytestSession boots the full game stack in headless mode, exercises a
// configurable set of ticks, checks GameSystemsRegistry health, and emits
// a structured result suitable for CI and TestHarness use.

#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <functional>

namespace NovaForge::App
{

/// Configuration for one playtest run.
struct PlaytestConfig
{
    int         tickCount       = 60;     ///< How many simulation ticks to run
    float       deltaTime       = 1.0f / 60.0f;
    bool        loadSave        = false;  ///< Try to load the save named saveName
    std::string saveName;
    std::string scenarioTag;              ///< Optional label for log output
    bool        abortOnFirstError = true; ///< Stop tick loop on first system error
};

/// Result produced by PlaytestSession::Run().
struct PlaytestResult
{
    bool        success     = false;
    int         ticksRun    = 0;
    int         errorCount  = 0;
    std::vector<std::string> errors;
    std::vector<std::string> log;

    /// Suitable as a process exit code: 0 = success, 1 = failure.
    int ExitCode() const { return success ? 0 : 1; }

    /// Multi-line human-readable summary.
    std::string Format() const;
};

/// Callback invoked once per tick; return false to abort early.
using PlaytestTickCallback = std::function<bool(int tickIndex, float dt)>;

/// Runs the game in headless mode for a fixed number of ticks.
///
/// Usage:
///   PlaytestConfig cfg;
///   cfg.tickCount = 120;
///   cfg.scenarioTag = "VerticalSlice";
///
///   PlaytestSession session(cfg);
///   PlaytestResult result = session.Run();
///   return result.ExitCode();
class PlaytestSession
{
public:
    explicit PlaytestSession(const PlaytestConfig& config = {});

    /// Optional per-tick callback for custom assertions.
    void SetTickCallback(PlaytestTickCallback cb) { m_tickCallback = std::move(cb); }

    /// Execute the full session.  Call only once per instance.
    PlaytestResult Run();

private:
    PlaytestConfig         m_config;
    PlaytestTickCallback   m_tickCallback;

    void EmitLog(PlaytestResult& result, const std::string& msg) const;
    void EmitError(PlaytestResult& result, const std::string& msg) const;
};

} // namespace NovaForge::App

// TestHarness.h
// NovaForge App — headless test harness for CI smoke-test runs.
//
// TestHarness assembles one or more PlaytestSession runs, collects results,
// and returns a final pass/fail decision.  It is the entry point for the
// `--mode playtest` launch path.

#pragma once
#include "PlaytestSession.h"
#include <string>
#include <vector>
#include <functional>

namespace NovaForge::App
{

/// Configuration for a single named test case within the harness.
struct TestCase
{
    std::string    name;
    PlaytestConfig sessionConfig;
    /// Optional setup/teardown hooks.
    std::function<void()> setup;
    std::function<void()> teardown;
};

/// Aggregated result across all test cases.
struct HarnessResult
{
    int  totalCases    = 0;
    int  passedCases   = 0;
    int  failedCases   = 0;
    std::vector<std::string> failedNames;
    std::vector<PlaytestResult> caseResults;

    bool AllPassed() const { return failedCases == 0 && totalCases > 0; }
    int  ExitCode()  const { return AllPassed() ? 0 : 1; }

    std::string Format() const;
};

/// Runs a suite of PlaytestSession instances and collects results.
class TestHarness
{
public:
    TestHarness() = default;

    /// Add a test case to the suite.
    void AddCase(TestCase tc);

    /// Add a minimal test case with just a name and tick count.
    void AddCase(const std::string& name, int tickCount = 10,
                 const std::string& scenarioTag = {});

    /// Run all registered test cases.
    HarnessResult RunAll();

    int CaseCount() const { return static_cast<int>(m_cases.size()); }

private:
    std::vector<TestCase> m_cases;
};

} // namespace NovaForge::App

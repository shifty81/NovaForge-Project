// TestHarness.cpp
// NovaForge App — test harness implementation.

#include "TestHarness.h"
#include <sstream>
#include <iostream>

namespace NovaForge::App
{

// ---------------------------------------------------------------------------
// HarnessResult::Format
// ---------------------------------------------------------------------------

std::string HarnessResult::Format() const
{
    std::ostringstream ss;
    ss << "=== TestHarness Results ===\n";
    ss << "Total  : " << totalCases  << "\n";
    ss << "Passed : " << passedCases << "\n";
    ss << "Failed : " << failedCases << "\n";
    if (!failedNames.empty())
    {
        ss << "Failed cases:\n";
        for (const auto& n : failedNames)
            ss << "  - " << n << "\n";
    }
    ss << "Result : " << (AllPassed() ? "PASS" : "FAIL") << "\n";
    return ss.str();
}

// ---------------------------------------------------------------------------
// AddCase
// ---------------------------------------------------------------------------

void TestHarness::AddCase(TestCase tc)
{
    m_cases.push_back(std::move(tc));
}

void TestHarness::AddCase(const std::string& name, int tickCount,
                           const std::string& scenarioTag)
{
    TestCase tc;
    tc.name = name;
    tc.sessionConfig.tickCount   = tickCount;
    tc.sessionConfig.scenarioTag = scenarioTag.empty() ? name : scenarioTag;
    m_cases.push_back(std::move(tc));
}

// ---------------------------------------------------------------------------
// RunAll
// ---------------------------------------------------------------------------

HarnessResult TestHarness::RunAll()
{
    HarnessResult harness;
    harness.totalCases = static_cast<int>(m_cases.size());

    for (auto& tc : m_cases)
    {
        std::cout << "[TestHarness] Running: " << tc.name << "\n";

        if (tc.setup)   tc.setup();

        PlaytestSession session(tc.sessionConfig);
        PlaytestResult  r = session.Run();
        harness.caseResults.push_back(r);

        if (tc.teardown) tc.teardown();

        if (r.success)
        {
            ++harness.passedCases;
            std::cout << "[TestHarness] PASS: " << tc.name << "\n";
        }
        else
        {
            ++harness.failedCases;
            harness.failedNames.push_back(tc.name);
            std::cout << "[TestHarness] FAIL: " << tc.name << "\n";
        }
    }

    std::cout << harness.Format();
    return harness;
}

} // namespace NovaForge::App

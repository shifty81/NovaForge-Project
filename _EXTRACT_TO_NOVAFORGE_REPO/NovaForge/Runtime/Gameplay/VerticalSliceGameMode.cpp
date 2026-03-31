#include "VerticalSliceGameMode.h"

// TODO: Replace with actual event bus / service locator includes.

namespace Runtime::Gameplay
{
VerticalSliceGameMode::VerticalSliceGameMode()
: m_currentPhase(EVerticalSlicePhase::Booting)
{
}

VerticalSliceGameMode::~VerticalSliceGameMode() = default;

bool VerticalSliceGameMode::Initialize()
{
    // TODO:
    // - register gameplay services
    // - bind mission / save / UI event listeners
    // - load scenario defaults
    return true;
}

void VerticalSliceGameMode::Shutdown()
{
    // TODO:
    // - flush telemetry
    // - unregister listeners
    // - release runtime references
}

void VerticalSliceGameMode::Tick(float /*deltaSeconds*/)
{
    // TODO:
    // - phase-specific polling where event-driven flow is insufficient
    // - runtime fail-state monitoring
}

void VerticalSliceGameMode::StartScenario(const std::string& scenarioId)
{
    m_activeScenarioId = scenarioId;
    RequestPhaseTransition(EVerticalSlicePhase::InShip);
}

void VerticalSliceGameMode::RequestPhaseTransition(EVerticalSlicePhase nextPhase)
{
    if (!ValidatePhaseTransition(nextPhase))
    {
        return;
    }

    const EVerticalSlicePhase previous = m_currentPhase;
    EnterPhase(nextPhase);
    BroadcastPhaseChanged(previous, nextPhase);
}

EVerticalSlicePhase VerticalSliceGameMode::GetCurrentPhase() const
{
    return m_currentPhase;
}

bool VerticalSliceGameMode::ValidatePhaseTransition(EVerticalSlicePhase /*nextPhase*/) const
{
    // TODO:
    // - encode valid state graph
    // - optionally consult mission / hazard / save gates
    return true;
}

void VerticalSliceGameMode::EnterPhase(EVerticalSlicePhase nextPhase)
{
    // TODO:
    // - execute enter hooks
    // - update UI state
    // - checkpoint where appropriate
    m_currentPhase = nextPhase;
}

void VerticalSliceGameMode::BroadcastPhaseChanged(EVerticalSlicePhase /*previous*/, EVerticalSlicePhase /*next*/)
{
    // TODO:
    // - send event to UI, telemetry, and debug tooling
}
}

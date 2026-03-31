#include "GameOrchestratorBoot.h"

namespace Runtime::Session
{
GameOrchestratorBoot::GameOrchestratorBoot()
    : m_bootPhase(EBootPhase::Idle)
{
}

GameOrchestratorBoot::~GameOrchestratorBoot() = default;

void GameOrchestratorBoot::Initialize()
{
    // TODO:
    // - acquire GameOrchestrator and NovaForgeBootstrap references
    // - reset boot phase to Idle
    m_bootPhase = EBootPhase::Idle;
}

bool GameOrchestratorBoot::RunBoot()
{
    // TODO:
    // - step through EBootPhase sequence via AdvancePhase
    // - invoke NovaForgeBootstrap for CoreSystems stage
    // - trigger world load, player spawn, and gameplay ready steps
    // - return false and set Error phase on any failure
    AdvancePhase(EBootPhase::CoreSystems);
    AdvancePhase(EBootPhase::WorldLoad);
    AdvancePhase(EBootPhase::PlayerSpawn);
    AdvancePhase(EBootPhase::GameplayReady);
    return true;
}

void GameOrchestratorBoot::Shutdown()
{
    // TODO:
    // - notify GameOrchestrator of teardown
    // - release held references
    m_bootPhase = EBootPhase::Idle;
}

EBootPhase GameOrchestratorBoot::GetBootPhase() const
{
    return m_bootPhase;
}

void GameOrchestratorBoot::AdvancePhase(EBootPhase next)
{
    // TODO:
    // - validate legal transition from m_bootPhase to next
    // - broadcast phase change event
    m_bootPhase = next;
}
}

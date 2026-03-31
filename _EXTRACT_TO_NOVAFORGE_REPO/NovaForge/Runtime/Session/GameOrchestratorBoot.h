#pragma once

namespace Runtime::Session
{
enum class EBootPhase
{
    Idle,
    CoreSystems,
    WorldLoad,
    PlayerSpawn,
    GameplayReady,
    Error
};

class GameOrchestratorBoot
{
public:
    GameOrchestratorBoot();
    ~GameOrchestratorBoot();

    void Initialize();
    bool RunBoot();
    void Shutdown();

    EBootPhase GetBootPhase() const;

private:
    void AdvancePhase(EBootPhase next);

private:
    EBootPhase m_bootPhase;
};
}

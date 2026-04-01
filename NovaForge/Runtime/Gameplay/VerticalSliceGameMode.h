#pragma once

#include <memory>
#include <string>

namespace Runtime::Gameplay
{
enum class EVerticalSlicePhase
{
Booting,
InShip,
AirlockCycling,
EVA,
SalvageActive,
HazardResponse,
ReturnTransit,
MissionComplete,
Debrief,
SavePending
};

class VerticalSliceGameMode
{
public:
    VerticalSliceGameMode();
    ~VerticalSliceGameMode();

    bool Initialize();
    void Shutdown();

    void Tick(float deltaSeconds);

    void StartScenario(const std::string& scenarioId);
    void RequestPhaseTransition(EVerticalSlicePhase nextPhase);

    EVerticalSlicePhase GetCurrentPhase() const;

private:
    bool ValidatePhaseTransition(EVerticalSlicePhase nextPhase) const;
    void EnterPhase(EVerticalSlicePhase nextPhase);
    void BroadcastPhaseChanged(EVerticalSlicePhase previous, EVerticalSlicePhase next);

private:
    EVerticalSlicePhase m_currentPhase;
    std::string m_activeScenarioId;
};
}

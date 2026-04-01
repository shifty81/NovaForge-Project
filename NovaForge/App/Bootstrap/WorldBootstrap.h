// WorldBootstrap.h
// NovaForge App — ordered world startup sequence (config → data → save → world → UI).

#pragma once
#include <functional>
#include <string>
#include <vector>

namespace NovaForge::App::Bootstrap
{

enum class EBootPhase : uint8_t
{
    NotStarted,
    LoadConfig,
    LoadDataRegistry,
    InitialiseGameplaySystems,
    LoadOrGenerateWorld,
    SpawnPlayer,
    BootstrapUI,
    BootstrapEditor,   ///< only in dev/editor mode
    Ready,
    Failed
};

/// Progress report for each phase.
struct BootPhaseResult
{
    EBootPhase  phase   = EBootPhase::NotStarted;
    bool        success = false;
    std::string message;
};

/// Delegate type for per-phase boot functions.
using PhaseDelegate = std::function<bool(std::string& outMessage)>;

class WorldBootstrap
{
public:
    WorldBootstrap();

    // ---- phase registration -----------------------------------------
    void RegisterPhase(EBootPhase phase, PhaseDelegate fn);

    // ---- execution --------------------------------------------------
    bool RunAll();

    bool RunPhase(EBootPhase phase);

    // ---- state queries ----------------------------------------------
    EBootPhase              CurrentPhase()   const { return m_currentPhase; }
    bool                    IsReady()        const { return m_currentPhase == EBootPhase::Ready; }
    bool                    HasFailed()      const { return m_currentPhase == EBootPhase::Failed; }
    const BootPhaseResult&  LastResult()     const { return m_lastResult; }
    std::vector<BootPhaseResult> GetHistory() const { return m_history; }

    // ---- editor mode ------------------------------------------------
    void SetEditorMode(bool enabled) { m_editorMode = enabled; }
    bool IsEditorMode()       const  { return m_editorMode; }

private:
    struct RegisteredPhase
    {
        EBootPhase    phase;
        PhaseDelegate fn;
    };

    EBootPhase                    m_currentPhase = EBootPhase::NotStarted;
    std::vector<RegisteredPhase>  m_phases;
    BootPhaseResult               m_lastResult;
    std::vector<BootPhaseResult>  m_history;
    bool                          m_editorMode  = false;

    /// Ordered sequence of phases to execute.
    static const std::vector<EBootPhase>& GetPhaseOrder();
};

} // namespace NovaForge::App::Bootstrap

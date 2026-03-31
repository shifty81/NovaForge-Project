// WorldBootstrap.cpp
// NovaForge App — ordered world startup sequence.

#include "WorldBootstrap.h"

#include <algorithm>

namespace NovaForge::App::Bootstrap
{

// ---------------------------------------------------------------------------
static const std::vector<EBootPhase> kPhaseOrder =
{
    EBootPhase::LoadConfig,
    EBootPhase::LoadDataRegistry,
    EBootPhase::InitialiseGameplaySystems,
    EBootPhase::LoadOrGenerateWorld,
    EBootPhase::SpawnPlayer,
    EBootPhase::BootstrapUI,
    EBootPhase::BootstrapEditor,   // skipped when m_editorMode == false
};

const std::vector<EBootPhase>& WorldBootstrap::GetPhaseOrder()
{
    return kPhaseOrder;
}

// ---------------------------------------------------------------------------

WorldBootstrap::WorldBootstrap()
    : m_currentPhase(EBootPhase::NotStarted)
{}

void WorldBootstrap::RegisterPhase(EBootPhase phase, PhaseDelegate fn)
{
    // Replace if already registered.
    for (auto& p : m_phases)
    {
        if (p.phase == phase)
        {
            p.fn = std::move(fn);
            return;
        }
    }
    m_phases.push_back({ phase, std::move(fn) });
}

bool WorldBootstrap::RunAll()
{
    for (EBootPhase phase : GetPhaseOrder())
    {
        // Skip editor-only phase in non-editor mode.
        if (phase == EBootPhase::BootstrapEditor && !m_editorMode)
            continue;

        if (!RunPhase(phase))
        {
            m_currentPhase = EBootPhase::Failed;
            return false;
        }
    }
    m_currentPhase = EBootPhase::Ready;
    return true;
}

bool WorldBootstrap::RunPhase(EBootPhase phase)
{
    m_currentPhase = phase;

    // Find registered delegate.
    for (const auto& rp : m_phases)
    {
        if (rp.phase == phase)
        {
            std::string msg;
            bool ok = rp.fn(msg);
            m_lastResult = { phase, ok, msg };
            m_history.push_back(m_lastResult);
            if (!ok) { m_currentPhase = EBootPhase::Failed; }
            return ok;
        }
    }

    // No delegate registered — treat as pass-through success.
    m_lastResult = { phase, true, "No delegate registered — skipped." };
    m_history.push_back(m_lastResult);
    return true;
}

} // namespace NovaForge::App::Bootstrap

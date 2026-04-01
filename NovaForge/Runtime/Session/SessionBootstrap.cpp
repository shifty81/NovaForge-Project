#include "SessionBootstrap.h"

namespace Runtime::Session
{
bool SessionBootstrap::Boot(const SessionBootstrapConfig& config)
{
// TODO:
// - initialize save/load domain access
// - inject deterministic seed
// - load scenario scene/prefabs
// - spawn player rig
// - start initial mission
return LoadScenario(config)
&& RestoreSaveIfNeeded(config)
&& SpawnPlayerRig(config)
&& StartMissionContext(config);
}

void SessionBootstrap::Shutdown()
{
    // TODO:
    // - ordered shutdown of runtime session services
}

bool SessionBootstrap::LoadScenario(const SessionBootstrapConfig& /*config*/)
{
    return true;
}

bool SessionBootstrap::RestoreSaveIfNeeded(const SessionBootstrapConfig& /*config*/)
{
    return true;
}

bool SessionBootstrap::SpawnPlayerRig(const SessionBootstrapConfig& /*config*/)
{
    return true;
}

bool SessionBootstrap::StartMissionContext(const SessionBootstrapConfig& /*config*/)
{
    return true;
}
}

// CombatSystem.cpp
#include "CombatSystem.h"

namespace NovaForge::Gameplay::Combat
{

void CombatSystem::initialise() {}
void CombatSystem::shutdown()   {}

CombatResult CombatSystem::resolve(const CombatEngagement& eng)
{
    CombatResult r;
    r.resolved    = true;
    r.damageDealt = eng.baseDamage;
    r.logEntry    = "CombatSystem::resolve stub";
    return r;
}

bool CombatSystem::isConsensusPvP(uint64_t, uint64_t) const { return false; }

void CombatSystem::registerKill(uint64_t, uint64_t) {}

} // namespace NovaForge::Gameplay::Combat

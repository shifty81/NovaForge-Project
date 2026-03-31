// CombatSystem.h
// NovaForge space/on-foot combat system.
// Handles PVE, consensual PVP, boarding, security responses, and damage resolution.

#pragma once
#include <cstdint>
#include <string>

namespace NovaForge::Gameplay::Combat
{

enum class CombatZone : uint8_t { Safe, LowSecurity, NullSecurity, Wardec };
enum class DamageType : uint8_t { Kinetic, Thermal, Explosive, EM };

struct CombatEngagement
{
    uint64_t    attackerId   = 0;
    uint64_t    defenderId   = 0;
    CombatZone  zone         = CombatZone::NullSecurity;
    DamageType  primaryDmg   = DamageType::Kinetic;
    float       baseDamage   = 0.0f;
    bool        isBoardingOp = false;
};

struct CombatResult
{
    bool    resolved     = false;
    float   damageDealt  = 0.0f;
    bool    targetKilled = false;
    std::string logEntry;
};

class CombatSystem
{
public:
    CombatSystem()  = default;
    ~CombatSystem() = default;

    void initialise();
    void shutdown();

    CombatResult resolve(const CombatEngagement& engagement);
    bool isConsensusPvP(uint64_t attackerId, uint64_t defenderId) const;
    void registerKill(uint64_t killerId, uint64_t victimId);
};

} // namespace NovaForge::Gameplay::Combat

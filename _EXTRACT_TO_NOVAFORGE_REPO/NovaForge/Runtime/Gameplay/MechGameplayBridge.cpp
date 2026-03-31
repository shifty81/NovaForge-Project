#include "MechGameplayBridge.h"

#include "Characters/Mech/MechPossessionSystem.h"

namespace Runtime::Gameplay
{
MechGameplayBridge::MechGameplayBridge()
    : m_initialized(false)
{
}

MechGameplayBridge::~MechGameplayBridge() = default;

bool MechGameplayBridge::Initialize()
{
    // TODO:
    // - acquire MechPossessionSystem reference from the service locator
    // - subscribe to vehicle entry/exit events on the gameplay event bus
    m_initialized = true;
    return true;
}

void MechGameplayBridge::Shutdown()
{
    // TODO:
    // - unsubscribe from vehicle entry/exit events
    // - release MechPossessionSystem reference
    m_initialized = false;
}

void MechGameplayBridge::RegisterCharacter(const std::string& characterId)
{
    // TODO:
    // - forward registration to MechPossessionSystem::RegisterCharacter
    (void)characterId;
}

bool MechGameplayBridge::OnEnterMechRequest(const std::string& characterId, const std::string& mechId)
{
    // TODO:
    // - validate that mechId refers to an unoccupied, reachable mech entity
    // - call MechPossessionSystem::EnterMech
    // - on success, broadcast an OnMechEntered event to the gameplay loop
    (void)characterId;
    (void)mechId;
    return false;
}

bool MechGameplayBridge::OnExitMechRequest(const std::string& characterId)
{
    // TODO:
    // - validate that characterId is currently possessing a mech
    // - call MechPossessionSystem::ExitMech
    // - on success, broadcast an OnMechExited event to the gameplay loop
    (void)characterId;
    return false;
}

bool MechGameplayBridge::IsInMech(const std::string& characterId) const
{
    // TODO:
    // - query MechPossessionSystem::FindState for characterId
    // - return MechPossessionState::bPossessing, or false if state is null
    (void)characterId;
    return false;
}

void MechGameplayBridge::Tick(float dt)
{
    // TODO:
    // - drive MechPossessionSystem::Tick
    // - process any queued entry/exit requests accumulated this frame
    (void)dt;
}
}

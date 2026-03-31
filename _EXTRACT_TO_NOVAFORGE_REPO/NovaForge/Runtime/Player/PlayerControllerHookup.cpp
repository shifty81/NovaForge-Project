#include "PlayerControllerHookup.h"

namespace Runtime::Player
{
PlayerControllerHookup::PlayerControllerHookup()
    : m_initialized(false)
{
}

PlayerControllerHookup::~PlayerControllerHookup() = default;

void PlayerControllerHookup::Initialize()
{
    // TODO:
    // - acquire CharacterSystem and PlayerController references
    // - register movement mode change callbacks
    m_initialized = true;
}

void PlayerControllerHookup::Shutdown()
{
    // TODO:
    // - deregister callbacks
    // - release system references
    m_initialized = false;
}

void PlayerControllerHookup::Tick(float /*dt*/)
{
    // TODO:
    // - poll pending movement mode changes
    // - flush dispatch queue to CharacterSystem
}

void PlayerControllerHookup::OnMovementModeChanged(const std::string& playerId, const std::string& mode)
{
    // TODO:
    // - translate mode string to ECharacterMovementMode
    // - forward to DispatchToCharacterSystem
    (void)playerId;
    (void)mode;
}

void PlayerControllerHookup::DispatchToCharacterSystem(const std::string& playerId)
{
    // TODO:
    // - call CharacterSystem::SetMovementMode for playerId
    // - notify PlayerController of confirmed mode switch
    (void)playerId;
}
}

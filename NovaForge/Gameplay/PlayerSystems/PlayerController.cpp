// PlayerController.cpp
#include "PlayerController.h"

namespace NovaForge::Gameplay::PlayerSystems
{

void PlayerController::initialise(uint64_t playerId)
{
    snapshot_          = {};
    snapshot_.playerId = playerId;
    snapshot_.state    = PlayerState::Idle;
    snapshot_.health   = 100.0f;
    snapshot_.shieldHP = 100.0f;
}

void PlayerController::shutdown() {}

void PlayerController::tick(float, const PlayerInput&) {}

PlayerSnapshot PlayerController::getSnapshot() const { return snapshot_; }

void PlayerController::setState(PlayerState newState) { snapshot_.state = newState; }

bool PlayerController::isDead() const { return snapshot_.state == PlayerState::Dead; }

void PlayerController::respawn(uint64_t locationId)
{
    snapshot_.state      = PlayerState::Idle;
    snapshot_.locationId = locationId;
    snapshot_.health     = 100.0f;
    snapshot_.shieldHP   = 100.0f;
}

} // namespace NovaForge::Gameplay::PlayerSystems

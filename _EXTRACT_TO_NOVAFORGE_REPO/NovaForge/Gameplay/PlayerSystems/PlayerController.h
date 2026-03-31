// PlayerController.h
// NovaForge player controller — movement, interaction, state management.

#pragma once
#include <cstdint>
#include <string>

namespace NovaForge::Gameplay::PlayerSystems
{

enum class PlayerState : uint8_t
{
    Idle, Flying, Docked, OnFoot, InCombat, Dead
};

struct PlayerInput
{
    float   thrustAxis      = 0.0f; ///< -1.0 to 1.0
    float   strafeAxis      = 0.0f;
    float   yawAxis         = 0.0f;
    bool    fireWeapon      = false;
    bool    requestDock     = false;
    bool    requestUndock   = false;
    bool    interact        = false;
};

struct PlayerSnapshot
{
    uint64_t    playerId    = 0;
    PlayerState state       = PlayerState::Idle;
    uint64_t    locationId  = 0; ///< sector/station/system ID
    float       health      = 100.0f;
    float       shieldHP    = 100.0f;
    std::string activeCraft;
};

class PlayerController
{
public:
    PlayerController()  = default;
    ~PlayerController() = default;

    void initialise(uint64_t playerId);
    void shutdown();
    void tick(float deltaSeconds, const PlayerInput& input);

    PlayerSnapshot getSnapshot() const;
    void setState(PlayerState newState);
    bool isDead() const;
    void respawn(uint64_t locationId);

private:
    PlayerSnapshot snapshot_;
};

} // namespace NovaForge::Gameplay::PlayerSystems

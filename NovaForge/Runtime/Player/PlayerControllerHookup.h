#pragma once

#include <string>

namespace Runtime::Player
{
class PlayerControllerHookup
{
public:
    PlayerControllerHookup();
    ~PlayerControllerHookup();

    void Initialize();
    void Shutdown();

    void Tick(float dt);

    void OnMovementModeChanged(const std::string& playerId, const std::string& mode);
    void DispatchToCharacterSystem(const std::string& playerId);

private:
    bool m_initialized;
};
}

#pragma once

#include <string>

namespace Runtime::Player
{
class IKAnimationBridge
{
public:
    IKAnimationBridge();
    ~IKAnimationBridge();

    bool Initialize();
    void Shutdown();

    void RegisterCharacter(const std::string& characterId);
    void UpdateFromCharacterState(const std::string& characterId);
    int  GetIKTargetCount(const std::string& characterId) const;
    void Tick(float dt);

private:
    bool m_initialized;
};
}

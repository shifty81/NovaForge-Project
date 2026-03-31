#pragma once

#include <string>

namespace Runtime::Gameplay
{
class MechGameplayBridge
{
public:
    MechGameplayBridge();
    ~MechGameplayBridge();

    bool Initialize();
    void Shutdown();

    void RegisterCharacter(const std::string& characterId);
    bool OnEnterMechRequest(const std::string& characterId, const std::string& mechId);
    bool OnExitMechRequest(const std::string& characterId);
    bool IsInMech(const std::string& characterId) const;
    void Tick(float dt);

private:
    bool m_initialized;
};
}

#include "IKAnimationBridge.h"

#include "Characters/IK/IKSystem.h"

namespace Runtime::Player
{
IKAnimationBridge::IKAnimationBridge()
    : m_initialized(false)
{
}

IKAnimationBridge::~IKAnimationBridge() = default;

bool IKAnimationBridge::Initialize()
{
    // TODO:
    // - acquire IKSystem reference from the service locator
    // - register with the animation pipeline event bus
    m_initialized = true;
    return true;
}

void IKAnimationBridge::Shutdown()
{
    // TODO:
    // - deregister from the animation pipeline event bus
    // - release IKSystem reference
    m_initialized = false;
}

void IKAnimationBridge::RegisterCharacter(const std::string& characterId)
{
    // TODO:
    // - forward registration to IKSystem::RegisterCharacter
    (void)characterId;
}

void IKAnimationBridge::UpdateFromCharacterState(const std::string& characterId)
{
    // TODO:
    // - fetch AuthoritativeCharacterState for characterId from the state cache
    // - call IKSystem::EvaluateFromCharacterState with the retrieved state
    (void)characterId;
}

int IKAnimationBridge::GetIKTargetCount(const std::string& characterId) const
{
    // TODO:
    // - query IKSystem::GetTargets and count entries matching characterId
    (void)characterId;
    return 0;
}

void IKAnimationBridge::Tick(float dt)
{
    // TODO:
    // - drive IKSystem::Tick
    // - push updated IK targets to the animation pose blender
    (void)dt;
}
}

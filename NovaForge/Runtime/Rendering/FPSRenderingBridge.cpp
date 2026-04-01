#include "FPSRenderingBridge.h"

#include "Characters/FPS/FPSPresentationSystem.h"

namespace Runtime::Rendering
{
FPSRenderingBridge::FPSRenderingBridge()
    : m_initialized(false)
{
}

FPSRenderingBridge::~FPSRenderingBridge() = default;

bool FPSRenderingBridge::Initialize()
{
    // TODO:
    // - acquire FPSPresentationSystem reference from the service locator
    // - register with the render pipeline's pre-render event
    m_initialized = true;
    return true;
}

void FPSRenderingBridge::Shutdown()
{
    // TODO:
    // - deregister from the render pipeline's pre-render event
    // - release FPSPresentationSystem reference
    m_initialized = false;
}

void FPSRenderingBridge::RegisterCharacter(const std::string& characterId)
{
    // TODO:
    // - forward registration to FPSPresentationSystem::RegisterCharacter
    // - allocate a per-character render slot in the FPS render queue
    (void)characterId;
}

void FPSRenderingBridge::SubmitForRendering(const std::string& characterId)
{
    // TODO:
    // - call FPSPresentationSystem::FindState to retrieve the current FPSPresentationState
    // - package camera offset, sway weight, and rig flags into a render command
    // - push the render command onto the FPS render queue for the current frame
    (void)characterId;
}

bool FPSRenderingBridge::IsVisible(const std::string& characterId) const
{
    // TODO:
    // - query FPSPresentationSystem::FindState for characterId
    // - return false if state is null or bFullBodyAwarenessEnabled is false
    (void)characterId;
    return false;
}

void FPSRenderingBridge::Tick(float dt)
{
    // TODO:
    // - drive FPSPresentationSystem::Tick
    // - call SubmitForRendering for every registered visible character
    (void)dt;
}
}

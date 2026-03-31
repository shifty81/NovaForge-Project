#include "RigController.h"

namespace Runtime::Player
{
RigController::RigController()
: m_movementMode(ERigMovementMode::InteriorWalk)
{
}

RigController::~RigController() = default;

void RigController::Tick(float deltaSeconds)
{
    UpdateInteractionTrace();
    UpdateToolState(deltaSeconds);
}

void RigController::SetMovementMode(ERigMovementMode mode)
{
    // TODO:
    // - switch locomotion profile
    // - notify animation / camera systems
    // - refresh UI hints
    m_movementMode = mode;
}

ERigMovementMode RigController::GetMovementMode() const
{
    return m_movementMode;
}

void RigController::BeginPrimaryInteraction()
{
    // TODO:
    // - activate focused interaction target
    // - route to equipped tool if applicable
}

void RigController::EndPrimaryInteraction()
{
    // TODO:
    // - end hold/progress interaction
}

void RigController::EquipTool(const std::string& toolId)
{
    // TODO:
    // - validate inventory ownership
    // - mount tool to hand socket
    // - update HUD icon
    m_equippedToolId = toolId;
}

const std::string& RigController::GetEquippedTool() const
{
    return m_equippedToolId;
}

void RigController::UpdateInteractionTrace()
{
    // TODO:
    // - perform view trace
    // - cache interactable target and prompt data
}

void RigController::UpdateToolState(float /*deltaSeconds*/)
{
    // TODO:
    // - tool-specific update
    // - charge/heat/progress handling
}
}

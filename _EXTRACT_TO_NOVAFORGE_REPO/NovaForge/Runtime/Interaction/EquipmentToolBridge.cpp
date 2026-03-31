#include "EquipmentToolBridge.h"

namespace Runtime::Interaction
{
EquipmentToolBridge::EquipmentToolBridge()
    : m_initialized(false)
{
}

EquipmentToolBridge::~EquipmentToolBridge() = default;

void EquipmentToolBridge::Initialize()
{
    // TODO:
    // - acquire EquipmentSystem and ToolInteractionShell references
    // - subscribe to equipment change events
    m_initialized = true;
}

void EquipmentToolBridge::OnToolEquipped(const std::string& playerId, const std::string& toolId)
{
    // TODO:
    // - notify ToolInteractionShell of newly active tool
    // - validate tool exists in EquipmentSystem inventory
    m_activePlayerId = playerId;
    m_activeToolId   = toolId;
}

void EquipmentToolBridge::OnToolUnequipped(const std::string& playerId)
{
    // TODO:
    // - clear ToolInteractionShell active tool
    // - broadcast unequip event to dependent systems
    if (m_activePlayerId == playerId)
    {
        m_activeToolId.clear();
    }
}

std::string EquipmentToolBridge::GetActiveToolId(const std::string& playerId) const
{
    // TODO:
    // - query EquipmentSystem directly as source of truth
    if (m_activePlayerId == playerId)
    {
        return m_activeToolId;
    }
    return {};
}
}

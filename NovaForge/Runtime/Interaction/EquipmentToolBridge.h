#pragma once

#include <string>

namespace Runtime::Interaction
{
class EquipmentToolBridge
{
public:
    EquipmentToolBridge();
    ~EquipmentToolBridge();

    void Initialize();

    void OnToolEquipped(const std::string& playerId, const std::string& toolId);
    void OnToolUnequipped(const std::string& playerId);

    std::string GetActiveToolId(const std::string& playerId) const;

private:
    std::string m_activePlayerId;
    std::string m_activeToolId;
    bool        m_initialized;
};
}

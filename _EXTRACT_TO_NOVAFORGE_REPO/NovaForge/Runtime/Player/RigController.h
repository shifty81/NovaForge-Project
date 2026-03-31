#pragma once

#include <string>

namespace Runtime::Player
{
enum class ERigMovementMode
{
InteriorWalk,
AirlockTransition,
EVAZeroG,
Ladder,
Seated
};

class RigController
{
public:
    RigController();
    ~RigController();

    void Tick(float deltaSeconds);

    void SetMovementMode(ERigMovementMode mode);
    ERigMovementMode GetMovementMode() const;

    void BeginPrimaryInteraction();
    void EndPrimaryInteraction();

    void EquipTool(const std::string& toolId);
    const std::string& GetEquippedTool() const;

private:
    void UpdateInteractionTrace();
    void UpdateToolState(float deltaSeconds);

private:
    ERigMovementMode m_movementMode;
    std::string m_equippedToolId;
};
}

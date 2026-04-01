#include "Gameplay/Endgame/EndgameGateSystem.h"
#include "Gameplay/Titan/TitanConstructionSystem.h"
#include <iostream>

bool EndgameGateSystem::Initialize(TitanConstructionSystem& InTitanSystem)
{
    TitanSystem = &InTitanSystem;
    std::cout << "[EndgameGate] Initialize\n";
    return true;
}

void EndgameGateSystem::RegisterGate(const EndgameGateState& Gate)
{
    Gates.push_back(Gate);
    std::cout << "[EndgameGate] Registered gate " << Gate.GateId << "\n";
}

bool EndgameGateSystem::TryChargeGate(const std::string& GateId, const std::string& TitanId)
{
    for (auto& Gate : Gates)
    {
        if (Gate.GateId == GateId)
        {
            if (Gate.bTitanRequired && (!TitanSystem || !TitanSystem->IsProjectComplete(TitanId)))
            {
                std::cout << "[EndgameGate] Titan incomplete, cannot charge gate\n";
                return false;
            }

            Gate.State = EGateState::Charging;
            std::cout << "[EndgameGate] Charging " << GateId << "\n";
            return true;
        }
    }
    return false;
}

bool EndgameGateSystem::TryTriggerJump(const std::string& GateId, const std::string& TitanId)
{
    for (auto& Gate : Gates)
    {
        if (Gate.GateId == GateId)
        {
            if (Gate.State != EGateState::Charging && Gate.State != EGateState::Ready)
            {
                std::cout << "[EndgameGate] Gate not ready for jump\n";
                return false;
            }

            if (Gate.bTitanRequired && (!TitanSystem || !TitanSystem->IsProjectComplete(TitanId)))
            {
                std::cout << "[EndgameGate] Titan incomplete, jump denied\n";
                return false;
            }

            Gate.State = EGateState::JumpTriggered;
            Gate.bJumpSuccessful = true;
            std::cout << "[EndgameGate] Jump triggered at " << GateId << "\n";
            return true;
        }
    }
    return false;
}

const EndgameGateState* EndgameGateSystem::FindGate(const std::string& GateId) const
{
    for (const auto& Gate : Gates)
    {
        if (Gate.GateId == GateId)
        {
            return &Gate;
        }
    }
    return nullptr;
}

void EndgameGateSystem::Tick(float)
{
    for (auto& Gate : Gates)
    {
        if (Gate.State == EGateState::Charging)
        {
            Gate.State = EGateState::Ready;
        }
        else if (Gate.State == EGateState::JumpTriggered)
        {
            Gate.State = EGateState::Completed;
        }
    }

    std::cout << "[EndgameGate] Gates=" << Gates.size() << "\n";
}

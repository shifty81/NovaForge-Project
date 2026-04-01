#pragma once

#include "Gameplay/Endgame/EndgameTypes.h"
#include <string>
#include <vector>

class TitanConstructionSystem;

class EndgameGateSystem
{
public:
    bool Initialize(TitanConstructionSystem& InTitanSystem);
    void RegisterGate(const EndgameGateState& Gate);
    bool TryChargeGate(const std::string& GateId, const std::string& TitanId);
    bool TryTriggerJump(const std::string& GateId, const std::string& TitanId);
    const EndgameGateState* FindGate(const std::string& GateId) const;
    void Tick(float DeltaTime);

private:
    TitanConstructionSystem* TitanSystem = nullptr;
    std::vector<EndgameGateState> Gates;
};

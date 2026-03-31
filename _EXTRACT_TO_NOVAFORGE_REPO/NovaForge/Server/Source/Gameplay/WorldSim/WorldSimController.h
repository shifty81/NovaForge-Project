#pragma once

#include "Gameplay/WorldSim/WorldSimTypes.h"

class FactionSystem;
class ContractBoardSystem;
class TradeSystem;

class WorldSimController
{
public:
    bool Initialize(FactionSystem& InFactions, ContractBoardSystem& InContracts, TradeSystem& InTrade);
    void Tick(float DeltaTime);

    const WorldSimState& GetState() const;

private:
    FactionSystem* Factions = nullptr;
    ContractBoardSystem* Contracts = nullptr;
    TradeSystem* Trade = nullptr;
    WorldSimState State;
};

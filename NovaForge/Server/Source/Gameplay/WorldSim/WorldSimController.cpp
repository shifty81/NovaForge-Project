#include "Gameplay/WorldSim/WorldSimController.h"
#include "Gameplay/Contracts/ContractBoardSystem.h"
#include "Gameplay/Factions/FactionSystem.h"
#include "Gameplay/Trade/TradeSystem.h"
#include <iostream>

bool WorldSimController::Initialize(FactionSystem& InFactions, ContractBoardSystem& InContracts, TradeSystem& InTrade)
{
    Factions = &InFactions;
    Contracts = &InContracts;
    Trade = &InTrade;
    std::cout << "[WorldSim] Initialize\n";
    return true;
}

void WorldSimController::Tick(float DeltaTime)
{
    ++State.TickCounter;
    std::cout << "[WorldSim] Tick=" << State.TickCounter
              << " DeltaTime=" << DeltaTime
              << " TimeScale=" << State.TimeScale << "\n";

    if (Factions) Factions->Tick(DeltaTime);
    if (Contracts) Contracts->Tick(DeltaTime);
    if (Trade) Trade->Tick(DeltaTime);
}

const WorldSimState& WorldSimController::GetState() const
{
    return State;
}

#include "Gameplay/Factions/FactionSystem.h"
#include <iostream>

bool FactionSystem::Initialize()
{
    std::cout << "[Factions] Initialize\n";
    return true;
}

void FactionSystem::RegisterFaction(const std::string& FactionId, int StartingReputation, int StartingInfluence)
{
    for (const auto& Faction : State.Standings)
    {
        if (Faction.FactionId == FactionId)
        {
            return;
        }
    }

    State.Standings.push_back({FactionId, StartingReputation, StartingInfluence});
    std::cout << "[Factions] Registered " << FactionId << "\n";
}

void FactionSystem::ModifyReputation(const std::string& FactionId, int Delta)
{
    for (auto& Faction : State.Standings)
    {
        if (Faction.FactionId == FactionId)
        {
            Faction.Reputation += Delta;
            std::cout << "[Factions] Reputation " << FactionId << " -> " << Faction.Reputation << "\n";
            return;
        }
    }
}

void FactionSystem::ModifyInfluence(const std::string& FactionId, int Delta)
{
    for (auto& Faction : State.Standings)
    {
        if (Faction.FactionId == FactionId)
        {
            Faction.Influence += Delta;
            std::cout << "[Factions] Influence " << FactionId << " -> " << Faction.Influence << "\n";
            return;
        }
    }
}

const FactionStanding* FactionSystem::FindFaction(const std::string& FactionId) const
{
    for (const auto& Faction : State.Standings)
    {
        if (Faction.FactionId == FactionId)
        {
            return &Faction;
        }
    }
    return nullptr;
}

void FactionSystem::Tick(float)
{
    std::cout << "[Factions] Count=" << State.Standings.size() << "\n";
}

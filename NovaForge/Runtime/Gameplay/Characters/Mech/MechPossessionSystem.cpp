#include "Gameplay/Characters/Mech/MechPossessionSystem.h"
#include <iostream>

bool MechPossessionSystem::Initialize()
{
    std::cout << "[MechPossession] Initialize\n";
    return true;
}

void MechPossessionSystem::RegisterCharacter(const std::string& CharacterId)
{
    States.push_back({CharacterId, "", false});
    std::cout << "[MechPossession] Registered " << CharacterId << "\n";
}

bool MechPossessionSystem::EnterMech(const std::string& CharacterId, const std::string& MechId)
{
    for (auto& State : States)
    {
        if (State.CharacterId == CharacterId)
        {
            State.MechId = MechId;
            State.bPossessing = true;
            std::cout << "[MechPossession] " << CharacterId << " entered mech " << MechId << "\n";
            return true;
        }
    }
    return false;
}

bool MechPossessionSystem::ExitMech(const std::string& CharacterId)
{
    for (auto& State : States)
    {
        if (State.CharacterId == CharacterId && State.bPossessing)
        {
            State.bPossessing = false;
            std::cout << "[MechPossession] " << CharacterId << " exited mech " << State.MechId << "\n";
            State.MechId.clear();
            return true;
        }
    }
    return false;
}

const MechPossessionState* MechPossessionSystem::FindState(const std::string& CharacterId) const
{
    for (const auto& State : States)
    {
        if (State.CharacterId == CharacterId)
        {
            return &State;
        }
    }
    return nullptr;
}

void MechPossessionSystem::Tick(float)
{
    std::cout << "[MechPossession] States=" << States.size() << "\n";
}

#pragma once

#include "Gameplay/Factions/FactionTypes.h"
#include <string>

class FactionSystem
{
public:
    bool Initialize();
    void RegisterFaction(const std::string& FactionId, int StartingReputation = 0, int StartingInfluence = 0);
    void ModifyReputation(const std::string& FactionId, int Delta);
    void ModifyInfluence(const std::string& FactionId, int Delta);
    const FactionStanding* FindFaction(const std::string& FactionId) const;
    void Tick(float DeltaTime);

private:
    FactionState State;
};

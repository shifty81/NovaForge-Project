#pragma once

#include "Gameplay/Characters/Mech/MechPossessionTypes.h"
#include <string>
#include <vector>

class MechPossessionSystem
{
public:
    bool Initialize();
    void RegisterCharacter(const std::string& CharacterId);
    bool EnterMech(const std::string& CharacterId, const std::string& MechId);
    bool ExitMech(const std::string& CharacterId);
    const MechPossessionState* FindState(const std::string& CharacterId) const;
    void Tick(float DeltaTime);

private:
    std::vector<MechPossessionState> States;
};

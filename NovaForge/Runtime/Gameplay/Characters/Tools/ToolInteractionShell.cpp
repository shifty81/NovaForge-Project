#include "Gameplay/Characters/Tools/ToolInteractionShell.h"
#include <iostream>

void MiningToolContract::OnEquip(const std::string& CharacterId)
{
    std::cout << "[ToolContract] Mining tool equipped by " << CharacterId << "\n";
}

void MiningToolContract::OnStartUse(const std::string& CharacterId)
{
    std::cout << "[ToolContract] Mining tool started by " << CharacterId << "\n";
}

void MiningToolContract::OnStopUse(const std::string& CharacterId)
{
    std::cout << "[ToolContract] Mining tool stopped by " << CharacterId << "\n";
}

void MiningToolContract::OnTickUse(const std::string& CharacterId, float DeltaTime)
{
    std::cout << "[ToolContract] Mining tick by " << CharacterId
              << " dt=" << DeltaTime << "\n";
}

ToolInteractionProfile MiningToolContract::GetInteractionProfile() const
{
    return {"mining_tool_basic", "anim_tool_equip_miner", "anim_tool_mining_loop", "anim_fingers_grip_heavy", true};
}

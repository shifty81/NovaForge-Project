#pragma once

#include "Gameplay/Characters/Tools/ToolInteractionTypes.h"

class MiningToolContract : public ICharacterToolContract
{
public:
    void OnEquip(const std::string& CharacterId) override;
    void OnStartUse(const std::string& CharacterId) override;
    void OnStopUse(const std::string& CharacterId) override;
    void OnTickUse(const std::string& CharacterId, float DeltaTime) override;
    ToolInteractionProfile GetInteractionProfile() const override;
};

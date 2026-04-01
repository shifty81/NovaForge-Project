#pragma once

#include <string>

struct ToolInteractionProfile
{
    std::string ToolId;
    std::string EquipAnimation;
    std::string UseAnimation;
    std::string FingerPose;
    bool bRequiresHandIK = true;
};

class ICharacterToolContract
{
public:
    virtual ~ICharacterToolContract() = default;
    virtual void OnEquip(const std::string& CharacterId) = 0;
    virtual void OnStartUse(const std::string& CharacterId) = 0;
    virtual void OnStopUse(const std::string& CharacterId) = 0;
    virtual void OnTickUse(const std::string& CharacterId, float DeltaTime) = 0;
    virtual ToolInteractionProfile GetInteractionProfile() const = 0;
};

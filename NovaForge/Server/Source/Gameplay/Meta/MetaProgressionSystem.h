#pragma once

#include "Gameplay/Meta/MetaProgressionTypes.h"
#include <string>

class MetaProgressionSystem
{
public:
    bool Initialize();
    void AddCredits(int Amount);
    void RegisterAsset(const OwnedAsset& Asset);
    const MetaProgressionState& GetState() const;
    void Tick(float DeltaTime);

private:
    MetaProgressionState State;
};

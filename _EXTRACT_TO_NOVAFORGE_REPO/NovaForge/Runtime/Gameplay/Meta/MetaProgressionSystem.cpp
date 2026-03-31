#include "Gameplay/Meta/MetaProgressionSystem.h"
#include <iostream>

bool MetaProgressionSystem::Initialize()
{
    std::cout << "[Meta] Initialize\n";
    return true;
}

void MetaProgressionSystem::AddCredits(int Amount)
{
    State.Credits += Amount;
    std::cout << "[Meta] Credits -> " << State.Credits << "\n";
}

void MetaProgressionSystem::RegisterAsset(const OwnedAsset& Asset)
{
    State.Assets.push_back(Asset);
    if (Asset.AssetType == "ship")
    {
        State.OwnedShipCount += 1;
    }
    else if (Asset.AssetType == "station")
    {
        State.OwnedStationCount += 1;
    }

    std::cout << "[Meta] Registered asset " << Asset.AssetId
              << " Type=" << Asset.AssetType << "\n";
}

const MetaProgressionState& MetaProgressionSystem::GetState() const
{
    return State;
}

void MetaProgressionSystem::Tick(float)
{
    std::cout << "[Meta] Ships=" << State.OwnedShipCount
              << " Stations=" << State.OwnedStationCount
              << " Credits=" << State.Credits << "\n";
}

#pragma once

#include <string>
#include <vector>

struct OwnedAsset
{
    std::string AssetId;
    std::string AssetType;
};

struct MetaProgressionState
{
    int Credits = 0;
    int OwnedShipCount = 0;
    int OwnedStationCount = 0;
    std::vector<OwnedAsset> Assets;
};

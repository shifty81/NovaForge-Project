#pragma once

#include <string>
#include <vector>

struct StorageSlot
{
    std::string ItemId;
    int Count = 0;
};

struct StorageContainer
{
    std::string ContainerId;
    int Capacity = 50;
    std::vector<StorageSlot> Slots;
};

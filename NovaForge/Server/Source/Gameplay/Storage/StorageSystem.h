#pragma once

#include "Gameplay/Storage/StorageTypes.h"
#include <string>
#include <vector>

class StorageSystem
{
public:
    bool Initialize();
    void RegisterContainer(const StorageContainer& Container);
    bool DepositItem(const std::string& ContainerId, const std::string& ItemId, int Count);
    bool WithdrawItem(const std::string& ContainerId, const std::string& ItemId, int Count);
    const StorageContainer* FindContainer(const std::string& ContainerId) const;
    void Tick(float DeltaTime);

private:
    std::vector<StorageContainer> Containers;
};

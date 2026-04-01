#include "Gameplay/Storage/StorageSystem.h"
#include <iostream>

bool StorageSystem::Initialize()
{
    std::cout << "[Storage] Initialize\n";
    return true;
}

void StorageSystem::RegisterContainer(const StorageContainer& Container)
{
    Containers.push_back(Container);
    std::cout << "[Storage] Registered " << Container.ContainerId << "\n";
}

bool StorageSystem::DepositItem(const std::string& ContainerId, const std::string& ItemId, int Count)
{
    for (auto& Container : Containers)
    {
        if (Container.ContainerId == ContainerId)
        {
            for (auto& Slot : Container.Slots)
            {
                if (Slot.ItemId == ItemId)
                {
                    Slot.Count += Count;
                    std::cout << "[Storage] Deposited " << ItemId << " x" << Count << "\n";
                    return true;
                }
            }

            if (static_cast<int>(Container.Slots.size()) < Container.Capacity)
            {
                Container.Slots.push_back({ItemId, Count});
                std::cout << "[Storage] Deposited new stack " << ItemId << " x" << Count << "\n";
                return true;
            }
        }
    }
    return false;
}

bool StorageSystem::WithdrawItem(const std::string& ContainerId, const std::string& ItemId, int Count)
{
    for (auto& Container : Containers)
    {
        if (Container.ContainerId == ContainerId)
        {
            for (auto It = Container.Slots.begin(); It != Container.Slots.end(); ++It)
            {
                if (It->ItemId == ItemId && It->Count >= Count)
                {
                    It->Count -= Count;
                    if (It->Count == 0)
                    {
                        Container.Slots.erase(It);
                    }
                    std::cout << "[Storage] Withdrew " << ItemId << " x" << Count << "\n";
                    return true;
                }
            }
        }
    }
    return false;
}

const StorageContainer* StorageSystem::FindContainer(const std::string& ContainerId) const
{
    for (const auto& Container : Containers)
    {
        if (Container.ContainerId == ContainerId)
        {
            return &Container;
        }
    }
    return nullptr;
}

void StorageSystem::Tick(float)
{
    std::cout << "[Storage] Containers=" << Containers.size() << "\n";
}

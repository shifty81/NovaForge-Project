#pragma once

#include "Gameplay/Manufacturing/ManufacturingTypes.h"
#include <string>
#include <vector>

class ManufacturingSystem
{
public:
    bool Initialize();
    void RegisterQueue(const ManufacturingQueue& Queue);
    void AddJob(const std::string& QueueId, const ManufacturingJob& Job);
    void Tick(float DeltaTime);
    const ManufacturingQueue* FindQueue(const std::string& QueueId) const;

private:
    std::vector<ManufacturingQueue> Queues;
};

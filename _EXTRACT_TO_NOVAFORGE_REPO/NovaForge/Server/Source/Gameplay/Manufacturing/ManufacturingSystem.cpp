#include "Gameplay/Manufacturing/ManufacturingSystem.h"
#include <iostream>

bool ManufacturingSystem::Initialize()
{
    std::cout << "[Manufacturing] Initialize\n";
    return true;
}

void ManufacturingSystem::RegisterQueue(const ManufacturingQueue& Queue)
{
    Queues.push_back(Queue);
    std::cout << "[Manufacturing] Registered queue " << Queue.QueueId << "\n";
}

void ManufacturingSystem::AddJob(const std::string& QueueId, const ManufacturingJob& Job)
{
    for (auto& Queue : Queues)
    {
        if (Queue.QueueId == QueueId)
        {
            Queue.Jobs.push_back(Job);
            std::cout << "[Manufacturing] Added job " << Job.JobId << " to " << QueueId << "\n";
            return;
        }
    }
}

void ManufacturingSystem::Tick(float DeltaTime)
{
    for (auto& Queue : Queues)
    {
        for (auto& Job : Queue.Jobs)
        {
            if (!Job.bCompleted)
            {
                Job.Progress += DeltaTime;
                if (Job.Progress >= Job.RequiredTime)
                {
                    Job.bCompleted = true;
                    std::cout << "[Manufacturing] Completed job " << Job.JobId << "\n";
                }
            }
        }
    }

    std::cout << "[Manufacturing] Queues=" << Queues.size() << "\n";
}

const ManufacturingQueue* ManufacturingSystem::FindQueue(const std::string& QueueId) const
{
    for (const auto& Queue : Queues)
    {
        if (Queue.QueueId == QueueId)
        {
            return &Queue;
        }
    }
    return nullptr;
}

// ManufacturingQueue.cpp
// NovaForge manufacturing — job queuing, progress tracking, and output delivery.

#include "Manufacturing/ManufacturingQueue.h"

#include <algorithm>

namespace NovaForge::Gameplay::Manufacturing
{

void ManufacturingQueue::initialise() {}
void ManufacturingQueue::shutdown()   { jobs_.clear(); }

void ManufacturingQueue::registerRecipe(const RecipeDefinition& recipe)
{
    recipes_.push_back(recipe);
}

uint64_t ManufacturingQueue::queueJob(uint64_t stationId, uint64_t playerId,
                                      const std::string& recipeId,
                                      uint32_t quantity)
{
    if (!findRecipe(recipeId)) return 0;

    ManufacturingJob job;
    job.jobId     = nextJobId_++;
    job.stationId = stationId;
    job.playerId  = playerId;
    job.recipeId  = recipeId;
    job.quantity  = quantity;
    job.status    = EJobStatus::Queued;
    job.progress  = 0.0f;
    jobs_.push_back(job);
    return job.jobId;
}

void ManufacturingQueue::tick(float deltaSeconds)
{
    for (auto& job : jobs_)
    {
        if (job.status == EJobStatus::Queued)
            job.status = EJobStatus::InProgress;

        if (job.status == EJobStatus::InProgress)
        {
            const RecipeDefinition* recipe = findRecipe(job.recipeId);
            if (!recipe) { job.status = EJobStatus::Cancelled; continue; }

            float step = deltaSeconds / (recipe->durationSeconds * job.quantity);
            job.progress += step;
            if (job.progress >= 1.0f)
            {
                job.progress = 1.0f;
                job.status   = EJobStatus::Completed;
            }
        }
    }
}

bool ManufacturingQueue::cancelJob(uint64_t jobId)
{
    for (auto& job : jobs_)
    {
        if (job.jobId == jobId &&
            (job.status == EJobStatus::Queued ||
             job.status == EJobStatus::InProgress))
        {
            job.status = EJobStatus::Cancelled;
            return true;
        }
    }
    return false;
}

std::vector<ManufacturingJob>
ManufacturingQueue::collectCompleted(uint64_t playerId)
{
    std::vector<ManufacturingJob> done;
    for (auto it = jobs_.begin(); it != jobs_.end(); )
    {
        if (it->playerId == playerId && it->status == EJobStatus::Completed)
        {
            // Stub: deliver output items to player inventory.
            // A full implementation would call InventorySystem::addItem() here.
            done.push_back(*it);
            it = jobs_.erase(it);
        }
        else { ++it; }
    }
    return done;
}

std::optional<ManufacturingJob>
ManufacturingQueue::findJob(uint64_t jobId) const
{
    for (const auto& j : jobs_)
        if (j.jobId == jobId) return j;
    return std::nullopt;
}

std::vector<ManufacturingJob>
ManufacturingQueue::listJobs(uint64_t playerId) const
{
    std::vector<ManufacturingJob> result;
    for (const auto& j : jobs_)
        if (j.playerId == playerId) result.push_back(j);
    return result;
}

const RecipeDefinition*
ManufacturingQueue::findRecipe(const std::string& recipeId) const
{
    for (const auto& r : recipes_)
        if (r.recipeId == recipeId) return &r;
    return nullptr;
}

} // namespace NovaForge::Gameplay::Manufacturing

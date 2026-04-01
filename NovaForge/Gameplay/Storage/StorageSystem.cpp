// StorageSystem.cpp
// NovaForge storage — per-station item depot, manufacturing jobs, module bays.

#include "Storage/StorageSystem.h"

#include <algorithm>

namespace NovaForge::Gameplay::Storage
{

void StorageSystem::initialise() {}
void StorageSystem::shutdown()
{
    depots_.clear();
    jobs_.clear();
    bays_.clear();
}

// ---- depot management -------------------------------------------------------

uint64_t StorageSystem::createDepot(uint64_t stationId, uint64_t ownerId,
                                     const InventoryLimits& limits)
{
    StorageDepot d;
    d.depotId   = nextDepotId_++;
    d.stationId = stationId;
    d.ownerId   = ownerId;
    d.limits    = limits;
    depots_.push_back(d);
    return d.depotId;
}

void StorageSystem::deleteDepot(uint64_t depotId)
{
    depots_.erase(
        std::remove_if(depots_.begin(), depots_.end(),
                       [depotId](const StorageDepot& d){ return d.depotId == depotId; }),
        depots_.end());
}

uint32_t StorageSystem::depositItems(uint64_t depotId,
                                      const std::string& itemId,
                                      uint32_t quantity)
{
    StorageDepot* d = getMutableDepot(depotId);
    if (!d || quantity == 0) return 0;

    for (auto& slot : d->slots)
    {
        if (slot.itemId == itemId)
        {
            slot.quantity += quantity;
            return quantity;
        }
    }
    if (d->slots.size() < d->limits.maxSlots)
    {
        d->slots.push_back({ itemId, quantity });
        return quantity;
    }
    return 0;
}

bool StorageSystem::withdrawItems(uint64_t depotId,
                                   const std::string& itemId,
                                   uint32_t quantity)
{
    StorageDepot* d = getMutableDepot(depotId);
    if (!d) return false;

    for (auto& slot : d->slots)
    {
        if (slot.itemId == itemId)
        {
            if (slot.quantity < quantity) return false;
            slot.quantity -= quantity;
            return true;
        }
    }
    return false;
}

uint32_t StorageSystem::queryItemCount(uint64_t depotId,
                                        const std::string& itemId) const
{
    for (const auto& d : depots_)
    {
        if (d.depotId != depotId) continue;
        for (const auto& slot : d.slots)
            if (slot.itemId == itemId) return slot.quantity;
    }
    return 0;
}

std::optional<const StorageDepot*>
StorageSystem::findDepot(uint64_t depotId) const
{
    for (const auto& d : depots_)
        if (d.depotId == depotId) return &d;
    return std::nullopt;
}

// ---- manufacturing job queue ------------------------------------------------

uint64_t StorageSystem::queueManufacturingJob(uint64_t stationId,
                                               uint64_t ownerId,
                                               const std::string& recipeId,
                                               const std::string& outputItemId,
                                               uint32_t quantity,
                                               float totalSeconds)
{
    ManufacturingJob job;
    job.jobId         = nextJobId_++;
    job.stationId     = stationId;
    job.ownerId       = ownerId;
    job.recipeId      = recipeId;
    job.outputItemId  = outputItemId;
    job.quantity      = quantity;
    job.totalSeconds  = totalSeconds;
    job.status        = EJobStatus::Queued;
    jobs_.push_back(job);
    return job.jobId;
}

bool StorageSystem::tickJobs(float deltaSeconds)
{
    bool anyCompleted = false;
    for (auto& job : jobs_)
    {
        if (job.status == EJobStatus::Queued)
            job.status = EJobStatus::InProgress;

        if (job.status == EJobStatus::InProgress)
        {
            job.progressSeconds += deltaSeconds;
            if (job.progressSeconds >= job.totalSeconds)
            {
                job.status = EJobStatus::Completed;
                anyCompleted = true;
            }
        }
    }
    return anyCompleted;
}

bool StorageSystem::collectCompletedJob(uint64_t jobId, uint64_t depotId)
{
    ManufacturingJob* job = getMutableJob(jobId);
    if (!job || job->status != EJobStatus::Completed) return false;

    depositItems(depotId, job->outputItemId, job->quantity);
    job->status = EJobStatus::Collected;
    return true;
}

std::vector<ManufacturingJob>
StorageSystem::listJobs(uint64_t ownerId) const
{
    std::vector<ManufacturingJob> result;
    for (const auto& j : jobs_)
        if (j.ownerId == ownerId) result.push_back(j);
    return result;
}

// ---- module bay -------------------------------------------------------------

uint64_t StorageSystem::createModuleBay(uint64_t stationId, uint32_t slotCount)
{
    ModuleBay bay;
    bay.bayId     = nextBayId_++;
    bay.stationId = stationId;
    for (uint32_t i = 0; i < slotCount; ++i)
        bay.slots.push_back({ nextSlotId_++, "", "" });
    bays_.push_back(bay);
    return bay.bayId;
}

bool StorageSystem::storeModule(uint64_t bayId,
                                 const std::string& moduleId,
                                 const std::string& moduleClass)
{
    ModuleBay* bay = getMutableBay(bayId);
    if (!bay) return false;
    for (auto& slot : bay->slots)
    {
        if (slot.moduleId.empty())
        {
            slot.moduleId    = moduleId;
            slot.moduleClass = moduleClass;
            return true;
        }
    }
    return false; // bay full
}

bool StorageSystem::retrieveModule(uint64_t bayId,
                                    const std::string& moduleId)
{
    ModuleBay* bay = getMutableBay(bayId);
    if (!bay) return false;
    for (auto& slot : bay->slots)
    {
        if (slot.moduleId == moduleId)
        {
            slot.moduleId.clear();
            slot.moduleClass.clear();
            return true;
        }
    }
    return false;
}

std::optional<const ModuleBay*>
StorageSystem::findBay(uint64_t bayId) const
{
    for (const auto& b : bays_)
        if (b.bayId == bayId) return &b;
    return std::nullopt;
}

// ---- helpers ----------------------------------------------------------------

StorageDepot* StorageSystem::getMutableDepot(uint64_t depotId)
{
    for (auto& d : depots_)
        if (d.depotId == depotId) return &d;
    return nullptr;
}

ManufacturingJob* StorageSystem::getMutableJob(uint64_t jobId)
{
    for (auto& j : jobs_)
        if (j.jobId == jobId) return &j;
    return nullptr;
}

ModuleBay* StorageSystem::getMutableBay(uint64_t bayId)
{
    for (auto& b : bays_)
        if (b.bayId == bayId) return &b;
    return nullptr;
}

} // namespace NovaForge::Gameplay::Storage

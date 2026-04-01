// StorageSystem.h
// NovaForge storage — per-station / per-base persistent item depot with
// manufacturing job queue and module storage bays.

#pragma once
#include "Inventory/InventoryTypes.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Storage
{

using namespace NovaForge::Gameplay::Inventory;

// --------------------------------------------------------------------------
// Storage depot (persistent item storage at a station/base)
// --------------------------------------------------------------------------

struct StorageDepot
{
    uint64_t                   depotId    = 0;
    uint64_t                   stationId  = 0;
    uint64_t                   ownerId    = 0;   ///< player or corp
    std::vector<InventorySlot> slots;
    InventoryLimits            limits;
    float                      currentMass   = 0.f;
    float                      currentVolume = 0.f;
};

// --------------------------------------------------------------------------
// Manufacturing job queue
// --------------------------------------------------------------------------

enum class EJobStatus : uint8_t { Queued, InProgress, Completed, Collected, Failed };

struct ManufacturingJob
{
    uint64_t    jobId      = 0;
    uint64_t    stationId  = 0;
    uint64_t    ownerId    = 0;
    std::string recipeId;
    std::string outputItemId;
    uint32_t    quantity       = 1;
    float       progressSeconds = 0.f;   ///< elapsed
    float       totalSeconds    = 60.f;  ///< total required
    EJobStatus  status         = EJobStatus::Queued;
};

// --------------------------------------------------------------------------
// Module storage bay (ships docked / held modules)
// --------------------------------------------------------------------------

struct ModuleBaySlot
{
    uint64_t    slotId    = 0;
    std::string moduleId;        ///< empty = slot free
    std::string moduleClass;
};

struct ModuleBay
{
    uint64_t                   bayId     = 0;
    uint64_t                   stationId = 0;
    std::vector<ModuleBaySlot> slots;
};

// --------------------------------------------------------------------------
// StorageSystem
// --------------------------------------------------------------------------

class StorageSystem
{
public:
    StorageSystem()  = default;
    ~StorageSystem() = default;

    void initialise();
    void shutdown();

    // ---- depot management -------------------------------------------
    uint64_t createDepot(uint64_t stationId, uint64_t ownerId,
                          const InventoryLimits& limits = {});
    void     deleteDepot(uint64_t depotId);

    uint32_t depositItems(uint64_t depotId,
                           const std::string& itemId, uint32_t quantity);
    bool     withdrawItems(uint64_t depotId,
                            const std::string& itemId, uint32_t quantity);
    uint32_t queryItemCount(uint64_t depotId, const std::string& itemId) const;

    std::optional<const StorageDepot*> findDepot(uint64_t depotId) const;

    // ---- manufacturing job queue ------------------------------------
    uint64_t queueManufacturingJob(uint64_t stationId, uint64_t ownerId,
                                    const std::string& recipeId,
                                    const std::string& outputItemId,
                                    uint32_t quantity,
                                    float totalSeconds = 60.f);
    bool     tickJobs(float deltaSeconds);  ///< advances all in-progress jobs
    bool     collectCompletedJob(uint64_t jobId, uint64_t depotId);
    std::vector<ManufacturingJob> listJobs(uint64_t ownerId) const;

    // ---- module bay -------------------------------------------------
    uint64_t createModuleBay(uint64_t stationId, uint32_t slotCount);
    bool     storeModule(uint64_t bayId, const std::string& moduleId,
                          const std::string& moduleClass);
    bool     retrieveModule(uint64_t bayId, const std::string& moduleId);
    std::optional<const ModuleBay*> findBay(uint64_t bayId) const;

private:
    std::vector<StorageDepot>       depots_;
    std::vector<ManufacturingJob>   jobs_;
    std::vector<ModuleBay>          bays_;
    uint64_t nextDepotId_ = 1;
    uint64_t nextJobId_   = 1;
    uint64_t nextBayId_   = 1;
    uint64_t nextSlotId_  = 1;

    StorageDepot* getMutableDepot(uint64_t depotId);
    ManufacturingJob* getMutableJob(uint64_t jobId);
    ModuleBay* getMutableBay(uint64_t bayId);
};

} // namespace NovaForge::Gameplay::Storage

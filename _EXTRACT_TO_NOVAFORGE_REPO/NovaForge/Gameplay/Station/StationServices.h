// StationServices.h
// NovaForge station — player/station interaction (storage, manufacturing, repair).

#pragma once
#include "Station/StationTypes.h"

#include <optional>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Station
{

class StationServices
{
public:
    StationServices()  = default;
    ~StationServices() = default;

    void initialise();
    void shutdown();

    /// Register a new station in the world.
    uint64_t registerStation(const std::string& name, uint32_t factionId);

    /// Add a service to an existing station.
    bool addService(uint64_t stationId, const StationService& service);

    /// Deposit items from player inventory into station storage.
    bool depositItems(uint64_t stationId, uint64_t playerId,
                      const std::string& itemId, uint32_t quantity);

    /// Withdraw items from station storage into player inventory.
    bool withdrawItems(uint64_t stationId, uint64_t playerId,
                       const std::string& itemId, uint32_t quantity);

    /// Request repair service for the player ship (deducts credits).
    bool repairShip(uint64_t stationId, uint64_t playerId, float& outCost);

    /// Request resupply of consumables.
    bool resupply(uint64_t stationId, uint64_t playerId, float& outCost);

    /// Query total stored quantity of an item at a station.
    uint32_t queryStorage(uint64_t stationId, const std::string& itemId) const;

    std::optional<const StationRecord*> findStation(uint64_t stationId) const;

private:
    std::vector<StationRecord> stations_;
    uint64_t nextStationId_ = 1;

    StationRecord* getMutableStation(uint64_t stationId);
};

} // namespace NovaForge::Gameplay::Station

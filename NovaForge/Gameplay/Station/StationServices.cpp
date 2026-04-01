// StationServices.cpp
// NovaForge station — player/station interaction (storage, manufacturing, repair).

#include "Station/StationServices.h"

#include <algorithm>

namespace NovaForge::Gameplay::Station
{

void StationServices::initialise() {}
void StationServices::shutdown()   { stations_.clear(); }

uint64_t StationServices::registerStation(const std::string& name,
                                          uint32_t factionId)
{
    StationRecord rec;
    rec.stationId   = nextStationId_++;
    rec.stationName = name;
    rec.factionId   = factionId;
    stations_.push_back(rec);
    return rec.stationId;
}

bool StationServices::addService(uint64_t stationId, const StationService& svc)
{
    StationRecord* rec = getMutableStation(stationId);
    if (!rec) return false;
    rec->services.push_back(svc);
    return true;
}

bool StationServices::depositItems(uint64_t stationId, uint64_t /*playerId*/,
                                   const std::string& itemId, uint32_t quantity)
{
    StationRecord* rec = getMutableStation(stationId);
    if (!rec) return false;

    for (auto& slot : rec->storage)
    {
        if (slot.itemId == itemId) { slot.quantity += quantity; return true; }
    }
    rec->storage.push_back({ itemId, quantity });
    return true;
}

bool StationServices::withdrawItems(uint64_t stationId, uint64_t /*playerId*/,
                                    const std::string& itemId, uint32_t quantity)
{
    StationRecord* rec = getMutableStation(stationId);
    if (!rec) return false;

    for (auto& slot : rec->storage)
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

bool StationServices::repairShip(uint64_t /*stationId*/, uint64_t /*playerId*/,
                                 float& outCost)
{
    // Stub: deduct credits proportional to hull damage.
    outCost = 0.0f;
    return true;
}

bool StationServices::resupply(uint64_t /*stationId*/, uint64_t /*playerId*/,
                               float& outCost)
{
    // Stub: restock standard consumables.
    outCost = 0.0f;
    return true;
}

uint32_t StationServices::queryStorage(uint64_t stationId,
                                       const std::string& itemId) const
{
    for (const auto& s : stations_)
    {
        if (s.stationId == stationId)
        {
            for (const auto& slot : s.storage)
                if (slot.itemId == itemId) return slot.quantity;
            return 0;
        }
    }
    return 0;
}

std::optional<const StationRecord*>
StationServices::findStation(uint64_t stationId) const
{
    for (const auto& s : stations_)
        if (s.stationId == stationId) return &s;
    return std::nullopt;
}

StationRecord* StationServices::getMutableStation(uint64_t stationId)
{
    for (auto& s : stations_)
        if (s.stationId == stationId) return &s;
    return nullptr;
}

} // namespace NovaForge::Gameplay::Station

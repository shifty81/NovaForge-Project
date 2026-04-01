// StationTypes.h
// NovaForge station — data types for station services, storage, and terminals.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Gameplay::Station
{

enum class EServiceType : uint8_t
{
    Repair,
    Resupply,
    Trading,
    Manufacturing,
    MissionBoard,
    FactionTerminal
};

struct StationService
{
    EServiceType type;
    bool         isAvailable = true;
    float        costModifier = 1.0f; ///< Price multiplier vs base cost
};

struct StorageSlot
{
    std::string itemId;
    uint32_t    quantity = 0;
};

struct StationRecord
{
    uint64_t                    stationId   = 0;
    std::string                 stationName;
    uint32_t                    factionId   = 0;
    std::vector<StationService> services;
    std::vector<StorageSlot>    storage;
    float                       storageCapacity = 1000.0f;
};

} // namespace NovaForge::Gameplay::Station

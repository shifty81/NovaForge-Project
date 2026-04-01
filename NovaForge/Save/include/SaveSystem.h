// SaveSystem.h
// NovaForge save — structured save/load for player, voxel chunks, economy, contracts.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace NovaForge::Save
{

// --------------------------------------------------------------------------
// Player save slot
// --------------------------------------------------------------------------

struct SavedPlayerState
{
    uint64_t    playerId    = 0;
    std::string playerName;
    float       posX = 0.f, posY = 0.f, posZ = 0.f;
    float       health       = 100.f;
    float       credits      = 0.f;
    uint32_t    activeSector = 0;
    std::string currentShipId;
};

// --------------------------------------------------------------------------
// Voxel slot — one blob per chunk
// --------------------------------------------------------------------------

struct SavedVoxelChunk
{
    std::string          chunkId;
    std::vector<uint8_t> data;   ///< serialised VoxelChunkEditor blob
};

// --------------------------------------------------------------------------
// Economy slot
// --------------------------------------------------------------------------

struct SavedTradeOrder
{
    uint64_t    orderId    = 0;
    uint64_t    stationId  = 0;
    std::string resourceId;
    float       price      = 0.f;
    uint32_t    quantity   = 0;
    bool        isBuyOrder = false;
};

struct SavedEconomyState
{
    std::vector<SavedTradeOrder> openOrders;
};

// --------------------------------------------------------------------------
// Contract slot
// --------------------------------------------------------------------------

struct SavedContractInstance
{
    uint64_t    instanceId  = 0;
    uint64_t    playerId    = 0;
    uint32_t    templateId  = 0;
    uint8_t     status      = 0;  ///< MissionStatus enum value
    std::string targetInfo;
};

struct SavedContractState
{
    std::vector<SavedContractInstance> instances;
};

// --------------------------------------------------------------------------
// Fleet save slot
// --------------------------------------------------------------------------

struct SavedFleetShip
{
    uint64_t    shipId   = 0;
    std::string shipName;
    std::string shipClass;
};

struct SavedFleetState
{
    uint64_t                    fleetId  = 0;
    uint64_t                    ownerId  = 0;
    std::string                 name;
    std::string                 sectorId;
    uint8_t                     status   = 0;
    std::vector<SavedFleetShip> ships;
};

// --------------------------------------------------------------------------
// Save metadata (slot header, used for save-select UI)
// --------------------------------------------------------------------------

struct SaveMetadata
{
    int         slot           = 0;
    std::string playerName;
    uint32_t    playTimeSeconds = 0;
    std::string locationLabel;
    uint32_t    saveVersion    = 1;
    bool        valid          = false;
};

// --------------------------------------------------------------------------
// Full save bundle
// --------------------------------------------------------------------------

struct SaveBundle
{
    uint32_t              formatVersion = 1;
    SavedPlayerState      player;
    std::vector<SavedVoxelChunk> voxelChunks;
    SavedEconomyState     economy;
    SavedContractState    contracts;
    std::vector<SavedFleetState> fleets;
    SaveMetadata          metadata;
    bool                  valid = false;
};

// --------------------------------------------------------------------------
// SaveSystem
// --------------------------------------------------------------------------

class SaveSystem
{
public:
    SaveSystem()  = default;
    ~SaveSystem() = default;

    bool initialise();
    void shutdown();

    // ---- save ----------------------------------------------------------
    bool savePlayer(const SavedPlayerState& player);
    bool saveVoxelChunk(const SavedVoxelChunk& chunk);
    bool saveEconomy(const SavedEconomyState& economy);
    bool saveContracts(const SavedContractState& contracts);
    bool saveFleet(const SavedFleetState& fleet);

    /// Flush the complete bundle to disk (slot 0 … kMaxSlots-1).
    bool flushToSlot(int slot);

    // ---- load ----------------------------------------------------------
    bool           loadFromSlot(int slot);
    const SaveBundle& getBundle() const { return m_bundle; }

    // ---- validation ----------------------------------------------------
    bool validateBundle() const;

    static constexpr int kMaxSlots = 5;

private:
    SaveBundle m_bundle;
    bool       m_dirty = false;
};

} // namespace NovaForge::Save

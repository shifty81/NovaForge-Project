// SaveSystem.cpp
// NovaForge save — structured save/load for player, voxel chunks, economy, contracts.

#include "SaveSystem.h"

namespace NovaForge::Save
{

bool SaveSystem::initialise()
{
    m_bundle        = {};
    m_bundle.valid  = false;
    m_dirty         = false;
    return true;
}

void SaveSystem::shutdown() {}

bool SaveSystem::savePlayer(const SavedPlayerState& player)
{
    m_bundle.player = player;
    m_dirty = true;
    return true;
}

bool SaveSystem::saveVoxelChunk(const SavedVoxelChunk& chunk)
{
    // Update existing chunk or append.
    for (auto& c : m_bundle.voxelChunks)
    {
        if (c.chunkId == chunk.chunkId)
        {
            c = chunk;
            m_dirty = true;
            return true;
        }
    }
    m_bundle.voxelChunks.push_back(chunk);
    m_dirty = true;
    return true;
}

bool SaveSystem::saveEconomy(const SavedEconomyState& economy)
{
    m_bundle.economy = economy;
    m_dirty = true;
    return true;
}

bool SaveSystem::saveContracts(const SavedContractState& contracts)
{
    m_bundle.contracts = contracts;
    m_dirty = true;
    return true;
}

bool SaveSystem::saveFleet(const SavedFleetState& fleet)
{
    for (auto& f : m_bundle.fleets)
    {
        if (f.fleetId == fleet.fleetId) { f = fleet; m_dirty = true; return true; }
    }
    m_bundle.fleets.push_back(fleet);
    m_dirty = true;
    return true;
}

bool SaveSystem::flushToSlot(int slot)
{
    if (slot < 0 || slot >= kMaxSlots) return false;
    // Stub: serialise m_bundle to a file path like "save_slot_N.dat".
    // In a real implementation, use a binary or JSON serialiser here.
    m_bundle.valid = true;
    m_dirty = false;
    return true;
}

bool SaveSystem::loadFromSlot(int slot)
{
    if (slot < 0 || slot >= kMaxSlots) return false;
    // Stub: deserialise from "save_slot_N.dat" into m_bundle.
    m_bundle.valid = true;
    return true;
}

bool SaveSystem::validateBundle() const
{
    if (!m_bundle.valid) return false;
    // Basic sanity: format version must match.
    return m_bundle.formatVersion == 1;
}

} // namespace NovaForge::Save

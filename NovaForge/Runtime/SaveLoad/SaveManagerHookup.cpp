#include "SaveManagerHookup.h"

namespace Runtime::SaveLoad
{
SaveManagerHookup::SaveManagerHookup()
    : m_initialized(false)
{
}

SaveManagerHookup::~SaveManagerHookup() = default;

void SaveManagerHookup::Initialize()
{
    // TODO:
    // - acquire SaveManager reference
    // - register world state change listener
    m_initialized = true;
}

bool SaveManagerHookup::SaveWorld(const std::string& slotName)
{
    // TODO:
    // - gather world serialization snapshot from active systems
    // - forward to SaveManager::Save with slotName
    // - return false on serialization or I/O failure
    m_lastSaveSlot = slotName;
    return true;
}

bool SaveManagerHookup::LoadWorld(const std::string& slotName)
{
    // TODO:
    // - request SaveManager to deserialize slotName
    // - rehydrate world state into active systems
    // - return false if slot does not exist or data is corrupt
    m_lastSaveSlot = slotName;
    return true;
}

void SaveManagerHookup::OnWorldStateChanged()
{
    // TODO:
    // - mark dirty flag for autosave scheduling
    // - optionally trigger incremental checkpoint
}

const std::string& SaveManagerHookup::GetLastSaveSlot() const
{
    return m_lastSaveSlot;
}
}

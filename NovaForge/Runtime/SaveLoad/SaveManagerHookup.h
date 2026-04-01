#pragma once

#include <string>

namespace Runtime::SaveLoad
{
class SaveManagerHookup
{
public:
    SaveManagerHookup();
    ~SaveManagerHookup();

    void Initialize();

    bool SaveWorld(const std::string& slotName);
    bool LoadWorld(const std::string& slotName);

    void OnWorldStateChanged();

    const std::string& GetLastSaveSlot() const;

private:
    std::string m_lastSaveSlot;
    bool        m_initialized;
};
}

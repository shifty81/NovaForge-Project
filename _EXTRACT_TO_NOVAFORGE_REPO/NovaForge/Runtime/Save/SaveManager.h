#pragma once

#include <string>
#include <vector>

struct SaveSlotInfo
{
    std::string SlotName;
    bool bExists = false;
};

class SaveManager
{
public:
    bool Initialize();
    bool Save(const std::string& SlotName);
    bool Load(const std::string& SlotName);
    void Shutdown();

    const std::vector<SaveSlotInfo>& GetSlots() const;

private:
    std::vector<SaveSlotInfo> Slots;
};

#pragma once

#include <string>
#include <vector>

struct UIScreenState
{
    bool bHUDVisible = true;
    bool bInventoryVisible = false;
    bool bContractsVisible = false;
    bool bStationServicesVisible = false;
    std::vector<std::string> Messages;
};

class VerticalSliceUI
{
public:
    bool Initialize();
    void Tick(float DeltaTime);
    void Shutdown();

    void ToggleInventory();
    void ToggleContracts();
    void ToggleStationServices();
    void ShowHUDMessage(const std::string& Message);

    const UIScreenState& GetState() const;

private:
    UIScreenState State;
};

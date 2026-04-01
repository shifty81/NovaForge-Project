#pragma once

#include <memory>
#include <string>

class VerticalSliceUI;
class SaveManager;
class DevOverlayState;
class IntegrationCoordinator;

class GameOrchestrator
{
public:
    GameOrchestrator();
    ~GameOrchestrator();
    bool Initialize();
    void Tick(float DeltaTime);
    void Shutdown();

    bool StartVerticalSliceSession();
    bool SaveGame(const std::string& SlotName);
    bool LoadGame(const std::string& SlotName);

private:
    std::unique_ptr<VerticalSliceUI> UI;
    std::unique_ptr<SaveManager> Saves;
    std::unique_ptr<DevOverlayState> DebugState;
    std::unique_ptr<IntegrationCoordinator> Integration;
    bool bInitialized = false;
};

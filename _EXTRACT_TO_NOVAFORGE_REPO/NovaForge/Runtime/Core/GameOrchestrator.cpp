#include "Core/GameOrchestrator.h"
#include "Debug/DevOverlayState.h"
#include "Integration/IntegrationCoordinator.h"
#include "Save/SaveManager.h"
#include "UI/VerticalSliceUI.h"
#include <iostream>

GameOrchestrator::GameOrchestrator() = default;

GameOrchestrator::~GameOrchestrator() = default;

bool GameOrchestrator::Initialize()
{
    UI = std::make_unique<VerticalSliceUI>();
    Saves = std::make_unique<SaveManager>();
    DebugState = std::make_unique<DevOverlayState>();
    Integration = std::make_unique<IntegrationCoordinator>();

    if (!UI->Initialize()) return false;
    if (!Saves->Initialize()) return false;
    if (!DebugState->Initialize()) return false;
    if (!Integration->Initialize()) return false;

    bInitialized = true;
    std::cout << "[GameOrchestrator] Initialized\n";
    return true;
}

void GameOrchestrator::Tick(float DeltaTime)
{
    if (!bInitialized) return;

    Integration->Tick(DeltaTime);
    DebugState->Tick(DeltaTime);
    UI->Tick(DeltaTime);
}

void GameOrchestrator::Shutdown()
{
    if (Integration) { Integration->Shutdown(); Integration.reset(); }
    if (DebugState) { DebugState->Shutdown(); DebugState.reset(); }
    if (Saves) { Saves->Shutdown(); Saves.reset(); }
    if (UI) { UI->Shutdown(); UI.reset(); }

    bInitialized = false;
    std::cout << "[GameOrchestrator] Shutdown\n";
}

bool GameOrchestrator::StartVerticalSliceSession()
{
    if (!bInitialized) return false;
    std::cout << "[GameOrchestrator] Starting vertical slice session\n";
    Integration->BootstrapPlayableLoop();
    UI->ShowHUDMessage("Vertical slice session started");
    return true;
}

bool GameOrchestrator::SaveGame(const std::string& SlotName)
{
    return Saves ? Saves->Save(SlotName) : false;
}

bool GameOrchestrator::LoadGame(const std::string& SlotName)
{
    return Saves ? Saves->Load(SlotName) : false;
}

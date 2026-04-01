#include "GameOrchestrator.h"
#include "DataRegistry.h"
#include "Renderer.h"
#include "SaveManager.h"
#include "RuntimeUIShell.h"
#include "EditorShell.h"
#include "World.h"
#include <iostream>

GameOrchestrator::GameOrchestrator() = default;

GameOrchestrator::~GameOrchestrator() = default;

bool GameOrchestrator::Initialize()
{
    Data = std::make_unique<DataRegistry>();
    Saves = std::make_unique<SaveManager>();
    RuntimeWorld = std::make_unique<World>();
    RenderSystem = std::make_unique<Renderer>();
    RuntimeUI = std::make_unique<RuntimeUIShell>();
    Editor = std::make_unique<EditorShell>();

    if (!Data->Initialize("Data")) return false;
    if (!Saves->Initialize()) return false;
    if (!RuntimeWorld->Initialize(*Data)) return false;
    if (!RenderSystem->Initialize()) return false;
    if (!RuntimeUI->Initialize()) return false;
    if (!Editor->Initialize()) return false;

    bInitialized = true;
    std::cout << "[GameOrchestrator] Initialized consolidated runtime\n";
    return true;
}

bool GameOrchestrator::StartVerticalSliceSession()
{
    if (!bInitialized) return false;
    std::cout << "[GameOrchestrator] Starting consolidated session\n";
    RuntimeUI->PushMessage("Session started");
    return true;
}

void GameOrchestrator::Tick(float DeltaTime)
{
    if (!bInitialized) return;

    RuntimeWorld->Tick(DeltaTime);
    Editor->Tick(DeltaTime);
    RuntimeUI->Tick(DeltaTime);
    RenderSystem->Render(*RuntimeWorld);
}

void GameOrchestrator::Shutdown()
{
    if (Editor) { Editor->Shutdown(); Editor.reset(); }
    if (RuntimeUI) { RuntimeUI->Shutdown(); RuntimeUI.reset(); }
    if (RenderSystem) { RenderSystem->Shutdown(); RenderSystem.reset(); }
    if (RuntimeWorld) { RuntimeWorld->Shutdown(); RuntimeWorld.reset(); }
    if (Saves) { Saves->Shutdown(); Saves.reset(); }
    if (Data) { Data->Shutdown(); Data.reset(); }

    bInitialized = false;
    std::cout << "[GameOrchestrator] Shutdown consolidated runtime\n";
}

#include "App.h"
#include "GameOrchestrator.h"
#include <iostream>

App::App() = default;

App::~App() = default;

bool App::Initialize()
{
    Orchestrator = std::make_unique<GameOrchestrator>();
    if (!Orchestrator->Initialize())
    {
        return false;
    }

    bRunning = true;
    return true;
}

void App::Run()
{
    constexpr float DeltaTime = 1.0f / 60.0f;
    constexpr int TickCount = 5;

    std::cout << "=== NovaForge Consolidated Boot ===\n";
    Orchestrator->StartVerticalSliceSession();

    for (int Tick = 0; Tick < TickCount && bRunning; ++Tick)
    {
        std::cout << "[App] Tick " << Tick << "\n";
        Orchestrator->Tick(DeltaTime);
    }
}

void App::Shutdown()
{
    if (Orchestrator)
    {
        Orchestrator->Shutdown();
        Orchestrator.reset();
    }

    bRunning = false;
    std::cout << "=== NovaForge Consolidated Shutdown ===\n";
}

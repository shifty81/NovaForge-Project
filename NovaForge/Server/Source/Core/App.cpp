#include "Core/App.h"
#include "Core/EngineKernel.h"
#include "Shared/Logging/MasterLogger.h"
#include <iostream>

App::~App() = default;

bool App::Initialize()
{
    atlastools::MasterLogger::Init("./Logs/server", "novaforge_server.log");
    MR_LOG_INFO("App::Initialize — starting NovaForge Runtime");

    Kernel = std::make_unique<EngineKernel>();
    if (!Kernel->Initialize())
    {
        MR_LOG_ERROR("App::Initialize — EngineKernel failed to initialise");
        return false;
    }

    bRunning = true;
    MR_LOG_INFO("App::Initialize — complete");
    return true;
}

void App::Run()
{
    constexpr int TickCount = 5;
    constexpr float DeltaTime = 1.0f / 60.0f;

    MR_LOG_INFO("App::Run — beginning " << TickCount << " ticks");
    std::cout << "=== NovaForge Runtime Boot ===\n";
    for (int TickIndex = 0; TickIndex < TickCount && bRunning; ++TickIndex)
    {
        MR_LOG_DEBUG("App::Run — tick " << TickIndex);
        std::cout << "[App] Tick " << TickIndex << "\n";
        Kernel->Tick(DeltaTime);
    }
    MR_LOG_INFO("App::Run — complete");
}

void App::Shutdown()
{
    MR_LOG_INFO("App::Shutdown — begin");
    if (Kernel)
    {
        Kernel->Shutdown();
        Kernel.reset();
    }

    bRunning = false;
    std::cout << "=== NovaForge Runtime Shutdown ===\n";
    MR_LOG_INFO("App::Shutdown — complete");
    atlastools::MasterLogger::Shutdown();
}

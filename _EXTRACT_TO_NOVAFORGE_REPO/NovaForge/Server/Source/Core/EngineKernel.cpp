#include "Core/EngineKernel.h"
#include "Data/DataRegistry.h"
#include "Rendering/Renderer.h"
#include "Tooling/ToolingSubsystem.h"
#include "World/World.h"
#include "Shared/Logging/MasterLogger.h"
#include <iostream>

EngineKernel::~EngineKernel() = default;

bool EngineKernel::Initialize()
{
    MR_LOG_INFO("EngineKernel::Initialize — begin");
    std::cout << "[Kernel] Initialize\n";

    Data = std::make_unique<DataRegistry>();
    if (!Data->Initialize("Data"))
    {
        MR_LOG_ERROR("EngineKernel::Initialize — DataRegistry failed");
        return false;
    }

    RuntimeWorld = std::make_unique<World>();
    if (!RuntimeWorld->Initialize(*Data))
    {
        MR_LOG_ERROR("EngineKernel::Initialize — World failed");
        return false;
    }

    RenderSystem = std::make_unique<Renderer>();
    RenderSystem->Initialize();

    Tooling = std::make_unique<ToolingSubsystem>();
    Tooling->Initialize();

    bInitialized = true;
    MR_LOG_INFO("EngineKernel::Initialize — complete");
    return true;
}

void EngineKernel::Tick(float DeltaTime)
{
    if (!bInitialized)
    {
        return;
    }

    Tooling->Tick(DeltaTime);
    RuntimeWorld->Tick(DeltaTime);
    RenderSystem->Render(*RuntimeWorld);
}

void EngineKernel::Shutdown()
{
    MR_LOG_INFO("EngineKernel::Shutdown — begin");
    std::cout << "[Kernel] Shutdown\n";

    if (Tooling)
    {
        Tooling->Shutdown();
        Tooling.reset();
    }

    if (RenderSystem)
    {
        RenderSystem->Shutdown();
        RenderSystem.reset();
    }

    if (RuntimeWorld)
    {
        RuntimeWorld->Shutdown();
        RuntimeWorld.reset();
    }

    if (Data)
    {
        Data->Shutdown();
        Data.reset();
    }

    bInitialized = false;
    MR_LOG_INFO("EngineKernel::Shutdown — complete");
}

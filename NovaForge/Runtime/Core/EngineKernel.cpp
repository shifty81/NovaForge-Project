#include "Core/EngineKernel.h"
#include "Data/DataRegistry.h"
#include "Rendering/Renderer.h"
#include "Tooling/ToolingSubsystem.h"
#include "World/World.h"
#include <iostream>

bool EngineKernel::Initialize()
{
    std::cout << "[Kernel] Initialize\n";

    Data = std::make_unique<DataRegistry>();
    if (!Data->Initialize("Data"))
    {
        return false;
    }

    RuntimeWorld = std::make_unique<World>();
    if (!RuntimeWorld->Initialize(*Data))
    {
        return false;
    }

    RenderSystem = std::make_unique<Renderer>();
    RenderSystem->Initialize();

    Tooling = std::make_unique<ToolingSubsystem>();
    Tooling->Initialize();

    bInitialized = true;
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
}

#pragma once

#include "Data/DataRegistry.h"
#include "Rendering/Renderer.h"
#include "Tooling/ToolingSubsystem.h"
#include "World/World.h"

#include <memory>

class EngineKernel
{
public:
    ~EngineKernel();
    bool Initialize();
    void Tick(float DeltaTime);
    void Shutdown();

private:
    std::unique_ptr<DataRegistry> Data;
    std::unique_ptr<World> RuntimeWorld;
    std::unique_ptr<Renderer> RenderSystem;
    std::unique_ptr<ToolingSubsystem> Tooling;
    bool bInitialized = false;
};

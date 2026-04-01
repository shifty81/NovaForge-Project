#pragma once

#include <memory>

class DataRegistry;
class World;
class Renderer;
class ToolingSubsystem;

class EngineKernel
{
public:
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

#pragma once

#include <memory>
#include <string>

class DataRegistry;
class SaveManager;
class World;
class Renderer;
class RuntimeUIShell;
class EditorShell;

class GameOrchestrator
{
public:
    GameOrchestrator();
    ~GameOrchestrator();
    bool Initialize();
    bool StartVerticalSliceSession();
    void Tick(float DeltaTime);
    void Shutdown();

private:
    std::unique_ptr<DataRegistry> Data;
    std::unique_ptr<SaveManager> Saves;
    std::unique_ptr<World> RuntimeWorld;
    std::unique_ptr<Renderer> RenderSystem;
    std::unique_ptr<RuntimeUIShell> RuntimeUI;
    std::unique_ptr<EditorShell> Editor;
    bool bInitialized = false;
};

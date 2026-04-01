#pragma once

#include <memory>

class GameOrchestrator;

class App
{
public:
    App();
    ~App();
    bool Initialize();
    void Run();
    void Shutdown();

private:
    std::unique_ptr<GameOrchestrator> Orchestrator;
    bool bRunning = false;
};

#pragma once

#include <memory>

class EngineKernel;

class App
{
public:
    ~App();
    bool Initialize();
    void Run();
    void Shutdown();

private:
    std::unique_ptr<EngineKernel> Kernel;
    bool bRunning = false;
};

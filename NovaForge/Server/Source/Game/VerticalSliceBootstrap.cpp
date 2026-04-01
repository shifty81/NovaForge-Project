#include "Core/GameOrchestrator.h"
#include <iostream>

int main()
{
    GameOrchestrator Orchestrator;

    if (!Orchestrator.Initialize())
    {
        std::cerr << "Failed to initialize GameOrchestrator\n";
        return 1;
    }

    Orchestrator.StartVerticalSliceSession();
    Orchestrator.Tick(1.0f / 60.0f);
    Orchestrator.SaveGame("slot_vertical_slice");
    Orchestrator.LoadGame("slot_vertical_slice");
    Orchestrator.Shutdown();
    return 0;
}

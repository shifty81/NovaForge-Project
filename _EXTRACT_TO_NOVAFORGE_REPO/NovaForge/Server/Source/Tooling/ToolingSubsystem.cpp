#include "Tooling/ToolingSubsystem.h"
#include <iostream>

bool ToolingSubsystem::Initialize()
{
    std::cout << "[Tooling] Initialize OverlayEnabled=" << (bOverlayEnabled ? "true" : "false") << "\n";
    return true;
}

void ToolingSubsystem::Tick(float)
{
    std::cout << "[Tooling] Overlay " << (bOverlayEnabled ? "Active" : "Hidden") << "\n";
}

void ToolingSubsystem::Shutdown()
{
    std::cout << "[Tooling] Shutdown\n";
}

void ToolingSubsystem::ToggleOverlay()
{
    bOverlayEnabled = !bOverlayEnabled;
    std::cout << "[Tooling] Overlay " << (bOverlayEnabled ? "Enabled" : "Disabled") << "\n";
}

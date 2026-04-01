#pragma once

class ToolingSubsystem
{
public:
    bool Initialize();
    void Tick(float DeltaTime);
    void Shutdown();
    void ToggleOverlay();

private:
    bool bOverlayEnabled = true;
};

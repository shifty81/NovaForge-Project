#pragma once

class World;

class Renderer
{
public:
    bool Initialize();
    void Render(const World& InWorld);
    void Shutdown();

private:
    int FrameCounter = 0;
};

#pragma once

class World;

class SystemScheduler
{
public:
    bool Initialize();
    void Tick(float DeltaTime, World& InWorld);
    void Shutdown();

private:
    int TickCounter = 0;
};

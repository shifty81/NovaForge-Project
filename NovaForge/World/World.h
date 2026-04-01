#pragma once

class DataRegistry;

class World
{
public:
    bool Initialize(DataRegistry& Data);
    void Tick(float DeltaTime);
    void Shutdown();
};

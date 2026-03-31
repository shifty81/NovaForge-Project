#include "World.h"
#include <iostream>

bool World::Initialize(DataRegistry&)
{
    std::cout << "[World] Initialize\n";
    return true;
}

void World::Tick(float)
{
    std::cout << "[World] Tick\n";
}

void World::Shutdown()
{
    std::cout << "[World] Shutdown\n";
}

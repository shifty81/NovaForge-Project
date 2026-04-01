#include "Rendering/Renderer.h"
#include "Modules/ModuleSubsystem.h"
#include "Voxel/VoxelSubsystem.h"
#include "World/World.h"
#include <iostream>

bool Renderer::Initialize()
{
    std::cout << "[Renderer] Initialize\n";
    return true;
}

void Renderer::Render(const World& InWorld)
{
    ++FrameCounter;
    std::cout << "[Renderer] Frame=" << FrameCounter
              << " World=" << InWorld.GetWorldName()
              << " Chunks=" << InWorld.GetVoxelSubsystem().GetChunkCount()
              << " Modules=" << InWorld.GetModuleSubsystem().GetModuleCount()
              << "\n";
}

void Renderer::Shutdown()
{
    std::cout << "[Renderer] Shutdown\n";
}

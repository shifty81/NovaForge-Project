cmake_minimum_required(VERSION 3.16)
project(MasterRepoPhase2Runtime LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(MasterRepoPhase2Runtime
    Source/main.cpp
    Source/Core/App.cpp
    Source/Core/EngineKernel.cpp
    Source/World/World.cpp
    Source/World/SystemScheduler.cpp
    Source/Entity/EntityRegistry.cpp
    Source/Entity/ComponentRegistry.cpp
    Source/Voxel/StructureRegistry.cpp
    Source/Voxel/VoxelSubsystem.cpp
    Source/Modules/ModuleRegistry.cpp
    Source/Modules/ModuleSubsystem.cpp
    Source/Rendering/Renderer.cpp
    Source/Tooling/ToolingSubsystem.cpp
    Source/Data/DataRegistry.cpp
)

target_include_directories(MasterRepoPhase2Runtime PRIVATE Source)

# MasterRepo Phase 2 Runtime

This pack provides the first true Phase 2 runtime slice.

## Included
- App bootstrap
- Engine kernel
- World + scheduler
- Entity registry
- Component registry
- Structure registry
- Voxel subsystem with test chunk
- Module registry + module subsystem with test module spawn
- Renderer shell
- Tooling subsystem shell
- Data registry that loads module JSON definitions

## Build
```bash
cmake -S . -B build
cmake --build build
./build/MasterRepoPhase2Runtime
```

## Current behavior
On startup, the runtime:
1. loads module definitions from `Data/Definitions/Modules`
2. creates a dev world
3. registers a test ship structure
4. creates a voxel chunk
5. spawns a test reactor module
6. ticks scheduler, voxel/module subsystems, tooling, and renderer

## What this is
A clean architecture-aligned slice.

## What this is not yet
- full ECS
- save/load wiring
- authority host
- real rendering backend
- real input routing
- real meshing
- real UI panels

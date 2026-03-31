# Build Rules

## Dependency Graph
- NovaForge.Core: no internal dependencies
- NovaForge.World -> NovaForge.Core
- NovaForge.Gameplay -> NovaForge.Core, NovaForge.World
- NovaForge.Rig -> NovaForge.Core, NovaForge.Gameplay
- NovaForge.Building -> NovaForge.Core, NovaForge.World, NovaForge.Gameplay
- NovaForge.Salvage -> NovaForge.Core, NovaForge.World, NovaForge.Gameplay
- NovaForge.Seasons -> NovaForge.Core
- NovaForge.Factions -> NovaForge.Core, NovaForge.Gameplay
- NovaForge.AI -> NovaForge.Core, NovaForge.World, NovaForge.Gameplay
- NovaForge.UI -> NovaForge.Core, NovaForge.Gameplay
- NovaForge.Client -> NovaForge.Core, NovaForge.Gameplay, NovaForge.Rig, NovaForge.UI
- NovaForge.Server -> NovaForge.Core, NovaForge.World, NovaForge.Gameplay, NovaForge.Rig, NovaForge.Building, NovaForge.Salvage, NovaForge.Seasons, NovaForge.Factions, NovaForge.AI
- NovaForge.Validation -> NovaForge.Core
- NovaForge.Tests -> may reference all production modules

## Rules
- no circular references
- no tooling modules
- no Atlas Suite shell dependencies
- no editor-only dependencies inside runtime modules
- validation runs before packaging
- packaging fails if validation fails
- prefer compile-safe incremental implementation

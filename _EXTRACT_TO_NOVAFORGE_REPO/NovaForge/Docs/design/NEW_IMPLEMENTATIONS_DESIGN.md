# Nova-Forge: Expeditions — New Implementations Design

> **Source archive:** `new implementations.md` (31 696-line design transcript).
> **Phased roadmap:** [`docs/IMPLEMENTATIONS_ROADMAP.md`](../IMPLEMENTATIONS_ROADMAP.md).
> **Existing systems:** [`docs/ROADMAP.md`](../ROADMAP.md) · [`docs/design/MASTER_DESIGN_BIBLE.md`](MASTER_DESIGN_BIBLE.md).

---

## Design Enforcement Rules (Non-Negotiable)

Every new system or feature **must** pass all three checks before implementation:

1. **Atlas-fit** — Does it belong naturally inside Atlas Engine's ECS, rendering, or audio systems?
2. **Simulation-first** — Does it function correctly without UI, audio, or player input?
3. **Lore-consistent** — Would a captain operating in the Nova-Forge universe accept this as real?

If it fails any one → it does not ship.

---

## Table of Contents

1. [AtlasForgeAI / SwissAgent — Expanded Module Architecture](#1-atlasforgeai--swissagent--expanded-module-architecture)
2. [Nova-Forge: Expeditions — Core Engine Tiers](#2-nova-forge-expeditions--core-engine-tiers)
3. [Rig Holo-Interface Panel](#3-rig-holo-interface-panel)
4. [Media Pipeline](#4-media-pipeline)
5. [SwissAgent Tool API Format](#5-swissagent-tool-api-format)
6. [CMake Build Template](#6-cmake-build-template)
7. [Repository Integration Map](#7-repository-integration-map)

---

## 1. AtlasForgeAI / SwissAgent — Expanded Module Architecture

### 1.1 Overview

**SwissAgent** (also referred to as **AtlasForgeAI** in the engine context) is the offline AI-powered development platform that underpins every automated workflow inside Nova-Forge: Expeditions. It is not a single program — it is a **modular repository of specialist agents, tools, and pipelines** that can be composed to build, test, generate assets, and ship software without internet access.

```
SwissAgent/
├─ core/            ← Agent loop, tool registry, plugin loader, permission system
├─ llm/             ← Local LLM abstraction (Ollama / llama.cpp / LM Studio)
├─ tools/           ← Registered tool implementations
├─ plugins/         ← Optional third-party or proprietary extensions
├─ workspace/       ← Active project context
├─ projects/        ← Multi-project management
├─ templates/       ← Project scaffold templates
├─ models/          ← Offline model weights
├─ configs/         ← Global and per-module configuration
├─ logs/            ← Structured logs per run
├─ cache/           ← Index, build cache, LLM prompt cache
└─ modules/         ← 34 specialist modules (see §1.3)
```

### 1.2 Agent Loop

```
┌─────────────────────────────────────────┐
│  User Prompt (CLI / IDE Panel / Voice)  │
└──────────────────┬──────────────────────┘
                   │
           ┌───────▼────────┐
           │   Plan with LLM │  ← project context injected
           └───────┬────────┘
                   │
          ┌────────▼─────────┐
          │  Identify Tools   │
          └────────┬─────────┘
                   │
         ┌─────────▼──────────┐
         │  Execute Tool(s)   │  ← filesystem / build / blender / git / …
         └─────────┬──────────┘
                   │
         ┌─────────▼──────────┐
         │  Parse Result /    │
         │  Handle Errors     │
         └─────────┬──────────┘
                   │
         ┌─────────▼──────────┐
         │  Update Project    │
         │  Files + Memory    │
         └─────────┬──────────┘
                   │
         ┌─────────▼──────────┐
         │  Done? → Stop      │
         │  No?  → Loop       │
         └────────────────────┘
```

The loop is implemented in `ai_dev/core/agent_loop.py` (Python) and mirrored by the C++ `AtlasForgeAI` core in `editor/ai/`.

### 1.3 Module Catalogue

Each module lives at `modules/<name>/` and provides:
- `module.json` — identity, version, dependencies
- `tools.json` — tool definitions loaded into the registry
- `src/` — implementation
- `include/` — headers (C++ modules)
- `scripts/` — utility scripts

| Module | Responsibility | Key Tools |
|--------|---------------|-----------|
| **filesystem** | Read, write, list, search, patch files; directory tree scanning | `read_file`, `write_file`, `list_dir`, `patch_file`, `search_code` |
| **git** | Clone, commit, branch, merge, diff, GitHub API integration | `git_clone`, `git_commit`, `git_push`, `git_diff`, `github_pr` |
| **zip** | Pack/unpack ZIP, 7z, tar.gz; batch compression | `zip_pack`, `zip_extract`, `tar_pack`, `archive_folder` |
| **build** | CMake, MSBuild, dotnet, Gradle, Make invocation; error parsing | `cmake_configure`, `cmake_build`, `msbuild_run`, `parse_errors` |
| **pipeline** | Multi-step task graphs: build → test → pack → release | `pipeline_run`, `pipeline_build`, `pipeline_test`, `pipeline_package` |
| **asset** | Import, convert, optimize, atlas-pack, deploy assets | `import_asset`, `convert_texture`, `pack_atlas`, `deploy_asset` |
| **render** | OpenGL/DX11/DX12/Vulkan pipeline generation and shader compilation | `generate_renderer`, `compile_shader`, `generate_pipeline` |
| **shader** | GLSL/HLSL generation, live recompile, hot-swap | `generate_shader`, `compile_glsl`, `hot_swap_shader` |
| **animation** | Clip/blend/state-machine generation, rig scripts | `generate_anim_clip`, `generate_blend_tree`, `export_animation` |
| **blender** | Blender CLI automation: model gen, rig, animation, export | `blender_gen_model`, `blender_gen_rig`, `blender_export_fbx`, `blender_run_script` |
| **image** | Procedural texture gen, atlas packing, resize, format conversion, Stable Diffusion | `gen_texture`, `pack_image_atlas`, `resize_image`, `stable_diffusion_gen` |
| **audio** | TTS voiceover, procedural SFX, music generation via AudioCraft/Bark | `gen_tts`, `gen_sfx`, `gen_music`, `encode_audio` |
| **ui** | Atlas UI widget/panel scaffold generation | `gen_panel`, `gen_widget`, `gen_hud_layout` |
| **editor** | Editor panel scaffolding, tool stubs, command bus wiring | `gen_editor_panel`, `gen_itool`, `gen_command` |
| **tile** | Tilemap creation, auto-tiling rules, Wang tile generation | `gen_tilemap`, `gen_autotile`, `export_tileset` |
| **network** | HTTP/WebSocket client wrappers, API call scaffolds | `http_get`, `http_post`, `websocket_connect` |
| **database** | SQLite/JSON DB schema gen, CRUD scaffolding | `gen_schema`, `db_query`, `db_migrate` |
| **script** | Lua, Python, C# runner and scaffolding | `run_lua`, `run_python`, `run_csharp`, `gen_script` |
| **debug** | Breakpoint helpers, log analysis, crash dump parsing | `parse_log`, `parse_crash`, `inject_debug_print` |
| **profile** | CPU/GPU profiling report generation, hotspot analysis | `run_profiler`, `parse_profile`, `gen_perf_report` |
| **binary** | Hex editing, binary patching, PE/ELF inspection | `hex_view`, `binary_patch`, `inspect_pe` |
| **resource** | Resource file registry, reference counting, hot-reload | `register_resource`, `reload_resource`, `gc_resources` |
| **package** | NuGet/pip/npm/vcpkg dependency management | `install_package`, `update_deps`, `lock_deps` |
| **installer** | NSIS/WiX/Inno Setup installer generation, auto-updater stubs | `gen_installer`, `gen_updater`, `sign_binary` |
| **doc** | README/API doc generation, inline comment extraction | `gen_readme`, `gen_api_docs`, `extract_comments` |
| **test** | Test scaffold generation, test runner, coverage parsing | `gen_test`, `run_tests`, `parse_coverage` |
| **ci** | GitHub Actions / GitLab CI YAML generation; local CI runner | `gen_ci_yaml`, `run_local_ci`, `trigger_ci` |
| **job** | Parallel job queue with priority and dependency graph | `queue_job`, `run_parallel`, `wait_job` |
| **cache** | LRU/disk cache for build artifacts, LLM responses, asset transforms | `cache_get`, `cache_set`, `invalidate_cache` |
| **memory** | Session memory, project context, long-term knowledge base | `store_memory`, `recall_memory`, `search_memory` |
| **security** | Allowed/blocked directory lists, sandbox enforcement, safe write/delete | `check_permission`, `sandbox_path`, `audit_access` |
| **api** | REST/GraphQL API client scaffold generation | `gen_api_client`, `call_api`, `mock_api` |
| **server** | Local dev server management (start/stop/reload) | `start_server`, `stop_server`, `reload_server` |
| **template** | Project scaffold templates: game, tool, plugin, ECS system | `apply_template`, `list_templates`, `create_template` |
| **index** | Symbol index, file index, cross-reference for large codebases | `find_symbol`, `find_class`, `find_ref`, `scan_project` |

### 1.4 Tool System — JSON Format

Every tool is described in a `tools.json` file inside its module:

```json
{
  "tools": [
    {
      "name": "write_file",
      "description": "Write content to a file at the given path, creating parent directories if needed.",
      "module": "filesystem",
      "permission": "write",
      "arguments": [
        { "name": "path",    "type": "string",  "required": true,  "description": "Absolute or workspace-relative file path." },
        { "name": "content", "type": "string",  "required": true,  "description": "UTF-8 file content." },
        { "name": "append",  "type": "boolean", "required": false, "description": "If true, append rather than overwrite." }
      ],
      "function": "filesystem.write_file"
    },
    {
      "name": "compile_cpp",
      "description": "Run CMake configure + build for a C++ project.",
      "module": "build",
      "permission": "execute",
      "arguments": [
        { "name": "project_dir",   "type": "string", "required": true },
        { "name": "build_type",    "type": "string", "required": false, "default": "Release" },
        { "name": "extra_flags",   "type": "string", "required": false }
      ],
      "function": "build.compile_cpp"
    }
  ]
}
```

### 1.5 Plugin System

Plugins extend SwissAgent without modifying core modules. They live in `/plugins/` and are loaded at startup.

```
plugins/
└── stable_diffusion/
    ├── plugin.json      ← { "name": "stable_diffusion", "version": "1.0.0", "requires": ["image"] }
    ├── tools.json       ← additional tool definitions injected into registry
    ├── scripts/
    │   └── sd_interface.py
    └── bin/             ← optional compiled binaries
```

**Bundled plugin stubs:**
- `stable_diffusion` — local SDXL/SD1.5 image generation
- `bark_tts` — offline neural TTS via Bark
- `audiocraft` — MusicGen / AudioGen for procedural music and SFX
- `ollama_backend` — Ollama LLM integration
- `blender_addon` — Blender Python addon bridge

### 1.6 LLM Integration

```
llm/
├── backends/
│   ├── ollama_backend.py       ← HTTP calls to local Ollama server
│   ├── llamacpp_backend.py     ← llama.cpp subprocess wrapper
│   └── api_backend.py          ← Optional cloud API fallback
├── llm_interface.py            ← Unified generate() / chat() / tool_call()
└── prompt_templates/           ← Per-language and per-task system prompts
```

Good offline models: DeepSeek Coder v2, Code Llama 70B, Qwen2.5 Coder, StarCoder2, Phi-4.

### 1.7 Workspace System

```
workspace/
├── workspace.json       ← active project list, shared deps
└── projects/
    └── <project-name>/
        ├── project.json ← { name, lang, build_system, root_dir }
        ├── src/
        ├── assets/
        ├── build/
        └── bin/
```

---

## 2. Nova-Forge: Expeditions — Core Engine Tiers

Nova-Forge: Expeditions is the combined game project produced by merging **NovaForge** (the space simulator), **AtlasForge** (the engine), and **SwissAgent/AtlasForgeAI** (the dev platform) into a single unified repository. The engine is organized into five vertical tiers.

```
Nova-Forge-Expeditions/
├─ Engine/              ← AtlasForge core (ECS, rendering, physics, audio, voxel)
├─ Game/                ← Nova-Forge gameplay ECS systems and scripts
├─ AI/                  ← AtlasForgeAI / SwissAgent integration
├─ Assets/              ← Models, textures, shaders, audio, prefabs
├─ Tools/               ← Editor plugins, hot-reload, overlay utilities
└─ Docs/                ← Design docs, procedural rules, AI workflows
```

---

### 2.1 Core Tier

The lowest layer — deterministic simulation primitives. All systems are **simulation-first**: they run without UI, audio, or player input.

#### Physics

| Class | Responsibility |
|-------|---------------|
| `RigidBodySystem` | Newtonian rigid-body dynamics, collision resolution, restitution |
| `SoftBodySystem` | Deformable meshes, cloth simulation, procedural deformation |
| `FluidSystem` | Water currents, buoyancy, surface and underwater simulation |
| `GasSystem` | Wind, pressure, temperature gradients, smoke, atmospheric gas mixing |
| `ZeroGPhysicsSystem` | Momentum conservation, thruster balancing, gyroscopic effects |
| `AtmosphericFlightSystem` | Lift, drag, turbulence, smooth space-atmosphere transitions |
| `ParticleSystem` | Explosion, fire propagation, dust clouds, debris simulation |
| `DestructibleSystem` | Voxel and mesh structural integrity, collapse, deformation |
| `ThermalSystem` | Heat propagation, thruster efficiency, chemical pressure reactions |

#### Voxels

| Class | Responsibility |
|-------|---------------|
| `VoxelWorld` | Chunk manager, voxel storage (16³ chunks), load/unload, serialization |
| `VoxelTerrain` | Procedural voxel terrain generation (noise + biome rules) |
| `Terraforming` | Player and AI-controlled terrain modification |
| `VoxelTools` | Place, remove, flood-fill, paint voxel blocks |
| `VoxelLightPropagation` | Dynamic lighting inside modular voxel structures |
| `VoxelPhysicsBlocks` | Fluid/gas/lava voxels with flow simulation |
| `VoxelMeshOptimizer` | LOD-based mesh simplification, greedy meshing |

#### Planet

| Class | Responsibility |
|-------|---------------|
| `PlanetGenerator` | Procedural planet surface from noise layers + biome map |
| `OrbitPhysics` | Newtonian orbits, gravity wells, multi-body Keplerian simulation |
| `ClimateSim` | Temperature, pressure, wind, precipitation, seasonal cycles |
| `ResourceNodeGen` | Surface outcroppings + underground deposit placement |
| `EcosystemSim` | Flora/fauna lifecycles, predator/prey cycles, migration AI |
| `TerraformingEngine` | Atmosphere manipulation, terrain shaping, ocean seeding |

#### Ships

| Class | Responsibility |
|-------|---------------|
| `ShipController` | Thruster input, flight model, docking approach curves |
| `ShipModules` | Modular hardpoint system — weapons, shields, sensors, engines |
| `FleetController` | Wing/squad coordination, formation keeping, doctrine execution |
| `DamagePropagation` | Module-to-module damage transfer, explosive chain reactions |

#### Character

| Class | Responsibility |
|-------|---------------|
| `CharacterRig` | Skeleton hierarchy, procedural IK setup |
| `AnimGraph` | Blend tree, state machine, animation clip playback |
| `IKFKBlending` | Smooth IK↔FK transitions for hands, feet, spine |
| `HoloInterfacePanel` | Wrist-mounted holo HUD (see §3) |
| `FacialExpression` | Blend shape morph targets driven by emotion state |
| `RagdollPhysics` | Procedural ragdoll on death/stun, secondary motion |

#### AI

| Class | Responsibility |
|-------|---------------|
| `CoreAI` | Intent-driven behaviour tree executor |
| `DialogueAI` | LLM-backed conversational NPC responses |
| `QuestGenAI` | Procedural quest template instantiation |
| `FactionAI` | Faction policy, territorial expansion, inter-faction relations |
| `SimulationAI` | Background universe tick — NPC schedules, idle behaviours |
| `EconomyAI` | Supply/demand agents, dynamic pricing, trade route selection |
| `GovernanceAI` | Tax policy, law enforcement, government collapse simulation |
| `LogisticsAI` | Cargo routing, supply chain optimization, hauler scheduling |
| `WorkerNode` | Individual NPC agent: mine, haul, patrol, dock, idle |

#### Assets

| Class | Responsibility |
|-------|---------------|
| `AssetBrowser` | In-engine asset database with filtering and tagging |
| `MarketplaceClient` | Optional online asset marketplace integration |
| `ModSDK` | JSON-based mod loading, validation, and dependency resolution |

---

### 2.2 Simulation Tier

Higher-level emergent simulation systems that compose the Core tier primitives.

#### Factions

| Class | Responsibility |
|-------|---------------|
| `Faction` | Identity, territory, resources, relationships |
| `CivSim` | Civilisation-scale lifecycle: expansion, stagnation, collapse |
| `Diplomacy` | Treaty negotiation, alliance management, war declaration |
| `CultureSim` | Cultural drift, tech diffusion, ideology propagation |

#### Economy

| Class | Responsibility |
|-------|---------------|
| `MarketSim` | Order-book market with realistic price discovery |
| `TradeAI` | Multi-hop trade route optimization |
| `ResourceFlow` | Production chain modelling from raw ore to finished goods |
| `Inflation` | Money supply dynamics, inflation/deflation simulation |

#### Quests & Story

| Class | Responsibility |
|-------|---------------|
| `QuestSystem` | Template instantiation, objective tracking, consequence chains |
| `Narrative` | Emergent story arc management — rising action, climax, resolution |
| `LoreGenerator` | Procedural in-universe books, inscriptions, faction histories |
| `EventLink` | Binds world events to quest triggers and story beats |

#### Combat

| Class | Responsibility |
|-------|---------------|
| `BattleSim` | Fleet engagement simulation (server tick) |
| `FleetAI` | Tactical maneuvering, target prioritization, retreat logic |
| `TacticsAI` | Doctrine execution, flanking, resource denial |

#### Colony & RTS

| Class | Responsibility |
|-------|---------------|
| `Colony` | Population, morale, infrastructure, resource consumption |
| `ResourceUse` | Per-colony supply/demand loop, stockpile management |
| `BuildingAI` | NPC construction scheduling, placement heuristics |
| `Territory` | Hexagonal control map, border dispute resolution |
| `FleetControl` | Player RTS control interface over multiple fleets |
| `AICommander` | Top-down strategic AI for NPC factions |

#### Events

| Class | Responsibility |
|-------|---------------|
| `CrisisAI` | Generates plausible galaxy-scale crises (plague, famine, war) |
| `Environmental` | Weather events, resource depletion, natural disasters |
| `DisasterSim` | Volcanic eruptions, tsunamis, meteor impacts, atmospheric loss |

---

### 2.3 Platform Tier

Infrastructure services that are game-agnostic.

| Class | Responsibility |
|-------|---------------|
| `Permissions` | File and API permission sandbox |
| `Sandbox` | Restricted execution environment for user scripts and mods |
| `Validator` | JSON schema validation for mod content |
| `Account` | Player account data model |
| `Auth` | Session token management, password hashing |
| `Profile` | Achievements, statistics, preferences |
| `Updater` | Delta-patch auto-updater |
| `CrashReporter` | Minidump generation, anonymized upload |
| `Telemetry` | Opt-in session analytics |
| `AntiCheat` | Server-side authority enforcement, sanity checks |
| `Packages` | Mod/DLC package installation and versioning |
| `Marketplace` | In-game marketplace listing and checkout |
| `ProjectHub` | Community project sharing and discovery |

---

### 2.4 Studio Tier

Multi-user collaborative development tools embedded in the editor.

| Class | Responsibility |
|-------|---------------|
| `MultiUserSession` | Real-time collaborative editing session management |
| `SyncManager` | Operational transform / CRDT for simultaneous edits |
| `ConflictResolver` | Three-way merge for ECS component data |
| `GitClient` | Embedded Git client in the editor |
| `AssetLock` | Pessimistic lock system to prevent conflicting asset edits |
| `TaskBoard` | Kanban-style task management integrated with the editor |
| `ReviewSystem` | Code/asset review workflow with inline comments |
| `BuildFarm` | Distributed build submission and result aggregation |
| `CollabChat` | Text + voice communication channel inside the editor |

---

### 2.5 DevAI Tier

AI-augmented development automation. Runs as part of the SwissAgent platform and surfaces through the editor overlay (Alt+F12).

| Class | Responsibility |
|-------|---------------|
| `AICoder` | LLM-driven code generation, scaffolding, and completion |
| `Refactor` | AI-assisted rename, extract-method, dead-code elimination |
| `Generator` | Full file generation from design prompts |
| `AssetDataset` | Curates training datasets from existing assets |
| `SimulationDataset` | Generates synthetic training data via simulation runs |
| `BuildGraph` | Dependency-aware incremental build orchestration |
| `Verifier` | Static analysis, test coverage gating, rule enforcement |
| `Fixer` | Auto-applies LLM-suggested fixes after build/test failures |
| `AIPipeline` | End-to-end: prompt → generate → build → test → fix → repeat |

---

### 2.6 SwissAgent IDE Integration

Surfaces the SwissAgent agent loop and tool system inside a Monaco-based editor panel and the Atlas Editor overlay.

| Component | Technology | Responsibility |
|-----------|-----------|---------------|
| **Monaco Editor** | VS Code's Monaco (web component) | Code editing with syntax highlighting, IntelliSense |
| **Open-WebUI** | Open-WebUI (Ollama front-end) | Chat interface for prompting the local LLM |
| `IDEBridge` | C++ ↔ Python IPC layer | Routes tool calls between the Atlas Editor and SwissAgent Python core |
| `IDEPanel` | Atlas UI panel | Hosts Monaco and Open-WebUI inside the editor docking system |
| `IDEWorkspace` | C++ | Syncs workspace.json with the Atlas project system |
| `IDEHotReload` | C++ file watcher + Python hook | Triggers reload on `.cpp`, `.h`, `.lua`, `.glsl`, `.json` save |
| `IDEBuildManager` | C++ | Wraps cmake/make calls, streams log output to `AIBuildLogPanel` |
| `IDEAIController` | C++ | Accepts AI suggestion diffs and applies them via `EditorCommandBus` |

**Overlay activation:** `Alt+F12` in the Atlas Editor or FPS playtest mode toggles the DevAI overlay.

---

### 2.7 Blender Integration Addon

A Python add-on installed into Blender that exposes Nova-Forge asset generation and export workflows directly from the Blender UI.

```
blender_addon/
├── __init__.py               ← Blender add-on registration
├── ProceduralGenerator.py    ← AI-driven mesh/texture generation from prompt
├── Exporter.py               ← FBX / GLB export with Nova-Forge conventions
├── PrefabLinker.py           ← Links Blender scenes to Nova-Forge prefab registry
└── AIIntegration.py          ← Calls SwissAgent API for prompt→model generation
```

| Script | Features |
|--------|---------|
| `ProceduralGenerator.py` | Generate ship hulls, terrain features, fauna models from text prompts via local SD + Blender geometry nodes |
| `Exporter.py` | Batch export with automatic LOD generation, material baking, and asset metadata embedding |
| `PrefabLinker.py` | Two-way sync between Blender scene and Nova-Forge prefab JSON; detects moved/deleted objects |
| `AIIntegration.py` | SwissAgent HTTP bridge; sends prompts, receives generated Blender Python scripts, executes them |

---

### 2.8 Engine Enhancements

#### Hot Reload

File-type aware reload without restarting the engine or the editor:

| File Type | Reload Method |
|-----------|--------------|
| `.lua` | `dofile()` re-execution |
| `.py` | `importlib.reload()` |
| `.glsl` / `.hlsl` | Recompile + shader program re-link |
| `.json` (config/data) | Re-parse, broadcast `DataReloadEvent` |
| Atlas UI panel `.cpp` | DLL hot-swap (Release builds: re-link shared object) |

#### LOD Streaming

| Class | Responsibility |
|-------|---------------|
| `LODManager` | Distance-based LOD level selection per entity |
| `AssetStreamer` | Background asset load/unload driven by camera proximity |

#### Rendering

| Class | Responsibility |
|-------|---------------|
| `RayTracing` | Hardware ray-tracing shadows and reflections (DX12 / Vulkan) |
| `GlobalIllumination` | Voxel-based GI, screen-space probes |
| `Volumetric` | Volumetric clouds, fog, atmospheric scattering |
| `ShaderGraph` | Visual node-based GLSL shader authoring |
| `ProceduralSky` | Day/night cycle, eclipses, auroras, nebula backdrop |

#### Audio

| Class | Responsibility |
|-------|---------------|
| `SpatialAudio` | HRTF-based 3D positional audio with occlusion, Doppler |
| `ProceduralAudio` | Procedural SFX synthesis (wind, water, engine hum) |
| `AdaptiveMusic` | Emergent music layer blending tied to game state |

#### Resource Virtualization

| Class | Responsibility |
|-------|---------------|
| `VirtualFileSystem` | Mounts mod packages and DLC as virtual directories |
| `ResourceVirtualization` | Lazy loading, reference counting, async streaming |

#### Networking

| Class | Responsibility |
|-------|---------------|
| `DistributedSim` | Partitions simulation across server nodes |
| `SyncManager` | Client-side prediction and server-reconciliation |

#### Scripting

| Class | Responsibility |
|-------|---------------|
| `LuaEngine` | Embedded LuaJIT runtime with Atlas API bindings |
| `PythonEngine` | Embedded CPython (or pybind11) with Atlas API bindings |

#### Serialization

| Class | Responsibility |
|-------|---------------|
| `SaveManager` | Incremental save/load using binary delta encoding |
| `VersionedSave` | Forward-compatible save files with migration callbacks |

---

### 2.9 Editor Tooling

#### Radial Menus

`ContextAwareRadial` — radial action menu that surfaces the most probable tools based on selected object type, recent history, and AI suggestions.

#### Visual Graphs

| Component | Responsibility |
|-----------|---------------|
| `NodeGraphEditor` | General-purpose node graph for AI behaviour trees and event graphs |
| `ShaderGraphEditor` | Drag-and-drop GLSL shader authoring with live preview |
| `TaskGraphEditor` | Pipeline stage visualization and editing |

#### Prefabs & Dependencies

| Component | Responsibility |
|-----------|---------------|
| `PrefabSystem` | Nested prefab instantiation with override support |
| `LiveDependencyGraph` | Real-time visualization of asset and code dependencies |

#### Performance & Debug

| Component | Responsibility |
|-----------|---------------|
| `ProfilerPanel` | Per-frame CPU/GPU timeline, sampled call stacks |
| `OptimizationOverlay` | Heatmap overlay showing expensive ECS components |
| `SimulationSandbox` | Isolated ECS world for offline simulation testing |
| `DebugOverlay` | Physics forces, AI decisions, voxel integrity, audio cones |

#### AI Overlay

| Component | Responsibility |
|-----------|---------------|
| `ToolingAIOverlay` | Context-aware suggestion panel (Alt+F12) |
| `SwissAgentPanel` | Embedded SwissAgent chat + tool runner inside editor |

#### Collaboration

| Component | Responsibility |
|-----------|---------------|
| `MultiUserSync` | Operational transform for simultaneous editing |
| `MergeAssistant` | AI-assisted three-way merge for ECS and scene data |

#### Recording & Undo

| Component | Responsibility |
|-----------|---------------|
| `SceneRecorder` | Record and play back editor sessions for tutorials |
| `UndoRedoStack` | Unlimited undo/redo using `UndoableCommandBus` |

---

### 2.10 Developer Tools

| Tool | Responsibility |
|------|---------------|
| `AutomatedTesting` | CI-triggered regression suite with AI-generated coverage |
| `ReplayAnalysis` | Replay server-side tick logs to diagnose simulation bugs |
| `ProceduralDebugging` | AI diagnoses procedural generation anomalies |
| `LiveProfiler` | In-engine sampling profiler with frame timeline |
| `AIDocumentation` | Extracts inline comments and generates API docs automatically |

---

## 3. Rig Holo-Interface Panel

### 3.1 Concept

The **Holo-Interface Panel** is the primary player HUD, visualized as a wrist-mounted holographic display that the player's Rig (powered exo-suit) projects in front of their left forearm. It replaces traditional 2D heads-up overlays with an in-universe diegetic interface — every interaction is lore-consistent.

### 3.2 HUD Zones

```
┌─────────────────────────────────────────┐
│  [GEAR / LOADOUT]   [CHARACTER STATS]   │
│  ─────────────────────────────────────  │
│  [AI INTERFACE]     [FLEET / SHIP LINK] │
│                                         │
│  [ACTIVE ALERTS]   [COMM LOG]           │
└─────────────────────────────────────────┘
```

| Zone | Content |
|------|---------|
| **Gear / Loadout** | Active weapon, consumables, module charge states, quick-swap slots |
| **Character Stats** | Health, energy, skill bonuses, status effects, Rig integrity |
| **AI Interface** | SwissAgent / NPC AI chat line, current AI directive, auto-pilot status |
| **Fleet / Ship Link** | Wing formation status, nearby ships, jump gate charge, warp countdown |
| **Active Alerts** | Environmental hazards, incoming weapons, proximity warnings |
| **Comm Log** | Recent faction transmissions, mission updates, NPC dialogue snippets |

### 3.3 ECS Components

```cpp
// In cpp_server/include/components/character_components.h

class HoloPanelState : public ecs::Component {
public:
    bool panel_visible = false;
    bool ai_active = false;
    float panel_opacity = 0.9f;
    std::string ai_message;
    std::string active_zone;   // "gear", "stats", "ai", "fleet", "alerts", "comms"
    COMPONENT_TYPE(HoloPanelState)
};
```

### 3.4 Connected Systems

| System | Connection |
|--------|-----------|
| `HoloPanelSystem` | Updates `HoloPanelState` per tick; routes input gestures |
| `InventorySystem` | Populates gear zone |
| `HealthSystem` | Feeds character stats zone |
| `AICompanionSystem` | Writes AI message slot |
| `FleetSystem` | Pushes wing formation data to fleet zone |
| `AlertSystem` | Raises warnings from collision, combat, and environmental systems |
| `CommunicationSystem` | Streams faction and NPC transmissions to comm log |

### 3.5 Rendering

The Holo-Interface Panel is rendered as a **world-space UI mesh** attached to the character's left forearm bone using the Atlas UI retained-mode system. It uses the EVE-style Photon Dark theme with additive blue glow shader to simulate holographic projection.

---

## 4. Media Pipeline

### 4.1 Overview

The **MediaPipeline** class provides a unified interface for offline AI-assisted generation of 2D textures, 3D models, audio, and video. It is called by SwissAgent tool handlers and by the Atlas Editor `AIOverlay`.

```
MediaPipeline/
├── stable_diffusion/       ← 2D image generation (SDXL / SD1.5 local)
├── blender_pipeline/       ← 3D model + render via Blender CLI
├── audio_pipeline/         ← TTS (Bark / Coqui), SFX (AudioCraft)
├── video_pipeline/         ← FFmpeg assembly from image sequences
└── shader_pipeline/        ← GLSL procedural texture generation
```

### 4.2 Class Interface

```python
class MediaPipeline:
    """Unified offline media generation pipeline."""

    def generate_2d_texture(self, name: str, prompt: str,
                             style: str = "sci-fi", size: tuple = (512, 512)) -> str:
        """Generate a 2D texture via Stable Diffusion. Returns output path."""

    def generate_3d_model(self, name: str, description: str,
                          format: str = "fbx") -> str:
        """Generate a 3D model via Blender Python script. Returns output path."""

    def generate_audio(self, type: str, name: str, text: str = "",
                       style: str = "ambient") -> str:
        """Generate TTS voiceover or procedural SFX. Returns output path.
           type: 'tts' | 'sfx' | 'music'
        """

    def generate_video(self, name: str, frames_source: str,
                       fps: int = 30, codec: str = "h264") -> str:
        """Assemble image sequence into video via FFmpeg. Returns output path."""

    def generate_shader_texture(self, name: str, glsl_template: str,
                                 resolution: int = 512) -> str:
        """Render a GLSL procedural texture offline. Returns output path."""
```

### 4.3 2D Generation — Stable Diffusion

```python
class StableDiffusionInterface:
    def __init__(self, model_path: str = "models/stable_diffusion"):
        self.model = self._load_model(model_path)

    def generate(self, prompt: str, style: str = "sci-fi",
                 negative_prompt: str = "", steps: int = 20,
                 size: tuple = (512, 512)) -> Image:
        """Run inference and return PIL Image."""
```

Supported backends:
- `diffusers` library (Hugging Face) — GPU-accelerated
- `stable-diffusion.cpp` — CPU fallback via ggml
- `automatic1111` REST endpoint — optional local web server

### 4.4 3D Generation — Blender CLI

```bash
# Agent calls Blender headlessly:
blender --background --python scripts/gen_model.py -- \
    --name "asteroid_type_a" \
    --description "craggy asteroid, sci-fi, metallic surface" \
    --format fbx \
    --output assets/models/
```

`gen_model.py` uses Blender geometry nodes + Blender's built-in randomize modifiers, driven by the description string tokenized into parameter seeds.

### 4.5 Audio Generation — TTS and SFX

```python
# TTS via Bark (offline neural TTS)
from bark import generate_audio, SAMPLE_RATE
audio = generate_audio("Warp drive charged. Initiating jump.")
soundfile.write("voiceover.wav", audio, SAMPLE_RATE)

# SFX via AudioCraft MusicGen
from audiocraft.models import AudioGen
model = AudioGen.get_pretrained("facebook/audiogen-medium")
output = model.generate(["laser blast, sci-fi, crisp"])
```

### 4.6 Video Assembly — FFmpeg

```bash
# FFmpeg called by video_pipeline module
ffmpeg -framerate 30 -i frames/frame_%04d.png \
       -vf "scale=1920:1080" \
       -c:v libx264 -pix_fmt yuv420p \
       output/cinematic.mp4
```

Agent tools wrap this with `media.generate_video(name, frames_dir, fps=30)`.

---

## 5. SwissAgent Tool API Format

### 5.1 Tool Call (LLM Output)

When the LLM decides a tool is needed it outputs a structured JSON block:

```json
{
  "tool_call": {
    "name": "write_file",
    "arguments": {
      "path": "cpp_server/src/systems/holo_panel_system.cpp",
      "content": "#include \"systems/holo_panel_system.h\"\n..."
    }
  }
}
```

The agent core parses this, looks up the tool in the registry, validates arguments against the schema, enforces permissions, and executes.

### 5.2 Tool Result

```json
{
  "tool_result": {
    "name": "write_file",
    "success": true,
    "output": "Wrote 847 bytes to cpp_server/src/systems/holo_panel_system.cpp"
  }
}
```

Errors return `"success": false` with `"error": "<message>"`, which is fed back to the LLM for self-correction.

### 5.3 Permission System

| Level | Allowed Operations |
|-------|--------------------|
| `read` | Read files within `allowed_dirs` |
| `write` | Write/create/delete files within `allowed_dirs` |
| `execute` | Run subprocesses (compiler, Blender, build tools) |
| `network` | HTTP requests (GitHub API, marketplace) |
| `admin` | Full access including `blocked_dirs` override |

Default policy: `allowed_dirs = [workspace_root]`, `blocked_dirs = [system32, /etc, /root]`.

### 5.4 Multi-Tool Step Example

```json
[
  { "tool_call": { "name": "read_file",    "arguments": { "path": "CMakeLists.txt" } } },
  { "tool_call": { "name": "write_file",   "arguments": { "path": "src/new_system.cpp", "content": "..." } } },
  { "tool_call": { "name": "compile_cpp",  "arguments": { "project_dir": ".", "build_type": "Debug" } } },
  { "tool_call": { "name": "run_tests",    "arguments": { "test_binary": "bin/test_systems" } } }
]
```

Steps run sequentially by default; the `job` module can parallelise independent steps.

---

## 6. CMake Build Template

Nova-Forge: Expeditions uses a unified top-level CMake that composes the server, client, engine, editor, and AI sub-projects.

```cmake
cmake_minimum_required(VERSION 3.20)
project(NovaForgeExpeditions VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Options ──────────────────────────────────────────────────────────────────
option(BUILD_SERVER        "Build cpp_server"            ON)
option(BUILD_CLIENT        "Build cpp_client"            ON)
option(BUILD_ENGINE        "Build Atlas Engine"          ON)
option(BUILD_EDITOR        "Build Atlas Editor"          ON)
option(BUILD_TESTS         "Build test binaries"         ON)
option(USE_STEAM_SDK       "Link against Steamworks SDK" OFF)
option(ENABLE_RAY_TRACING  "Enable DX12/Vulkan RT"       OFF)
option(ENABLE_VOXEL        "Enable voxel world system"   ON)
option(ENABLE_LUA          "Enable LuaJIT scripting"     ON)
option(ENABLE_PYTHON       "Enable Python scripting"     OFF)

# ── Sub-projects ─────────────────────────────────────────────────────────────
if(BUILD_ENGINE)
    add_subdirectory(engine)
endif()

if(BUILD_EDITOR)
    add_subdirectory(editor)
endif()

if(BUILD_SERVER)
    add_subdirectory(cpp_server)
endif()

if(BUILD_CLIENT)
    add_subdirectory(cpp_client)
endif()

# ── Shared interface target ───────────────────────────────────────────────────
add_library(novaforge_common INTERFACE)
target_include_directories(novaforge_common INTERFACE
    ${CMAKE_SOURCE_DIR}/cpp_common/include
)

# ── Tests ─────────────────────────────────────────────────────────────────────
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(atlas_tests)
endif()
```

### Server CMake (excerpt for new systems)

```cmake
# cpp_server/CMakeLists.txt (additions for new systems)
set(CORE_SOURCES
    # ... existing sources ...
    src/systems/holo_panel_system.cpp
    src/systems/voxel_build_system.cpp
    src/systems/climate_sim_system.cpp
    src/systems/ecosystem_sim_system.cpp
    src/systems/civ_sim_system.cpp
    src/systems/governance_system.cpp
    src/systems/ragdoll_system.cpp
    src/systems/crisis_ai_system.cpp
    src/systems/self_repair_ai_system.cpp
)
```

---

## 7. Repository Integration Map

```
NovaForge/                          ← current repo root
├─ cpp_server/                      ← authoritative simulation (ECS + new systems)
│   ├─ include/
│   │   ├─ components/
│   │   │   ├─ character_components.h    ← HoloPanelState, RagdollState, IKState
│   │   │   ├─ voxel_components.h        ← VoxelChunkState, VoxelTool, TerraformOp
│   │   │   ├─ simulation_components.h   ← CivState, GovernanceState, ClimateCellState
│   │   │   └─ ai_components.h           ← CrisisState, SelfRepairState
│   │   └─ systems/
│   │       ├─ holo_panel_system.h
│   │       ├─ voxel_build_system.h
│   │       ├─ climate_sim_system.h
│   │       ├─ ecosystem_sim_system.h
│   │       ├─ civ_sim_system.h
│   │       ├─ governance_system.h
│   │       ├─ ragdoll_system.h
│   │       ├─ crisis_ai_system.h
│   │       └─ self_repair_ai_system.h
│   └─ tests/
│       ├─ test_holo_panel_system.cpp
│       ├─ test_voxel_build_system.cpp
│       └─ ... (one per system)
│
├─ engine/                          ← Atlas Engine (PascalCase conventions)
│   ├─ RayTracing.*
│   ├─ GlobalIllumination.*
│   ├─ VolumetricClouds.*
│   ├─ ShaderGraph.*
│   ├─ LuaEngine.*
│   ├─ VoxelWorld.*
│   ├─ LODManager.*
│   └─ SpatialAudio.*
│
├─ editor/                          ← Atlas Editor (PascalCase conventions)
│   ├─ ai/
│   │   ├─ SwissAgentPanel.*
│   │   ├─ IDEBridge.*
│   │   └─ IDEAIController.*
│   └─ panels/
│       ├─ NodeGraphEditor.*
│       ├─ ShaderGraphEditor.*
│       └─ ProfilerPanel.*
│
├─ ai_dev/                          ← SwissAgent Python platform
│   ├─ core/
│   │   ├─ agent_loop.py
│   │   └─ tool_registry.py
│   ├─ modules/                     ← 34 modules (§1.3)
│   ├─ plugins/
│   └─ tools/
│       └─ media_pipeline.py
│
└─ blender_addon/                   ← Blender Python add-on
    ├─ __init__.py
    ├─ ProceduralGenerator.py
    ├─ Exporter.py
    ├─ PrefabLinker.py
    └─ AIIntegration.py
```

---

*Last updated: extracted and structured from `new implementations.md` (31 696-line design archive).*

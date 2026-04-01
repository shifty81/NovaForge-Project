# Nova-Forge: Expeditions — Implementations Roadmap

> Extracted from `new implementations.md` (31 696-line design archive).
> For deep design detail see [`docs/design/NEW_IMPLEMENTATIONS_DESIGN.md`](design/NEW_IMPLEMENTATIONS_DESIGN.md).
> For existing systems see [`docs/ROADMAP.md`](ROADMAP.md).

---

## Design Enforcement Rules (Non-Negotiable)

Every system **must** satisfy all three criteria before any work begins:

1. **Atlas-fit** — Belongs naturally in Atlas Engine's ECS, rendering, or audio.
2. **Simulation-first** — Operates correctly without UI, audio, or player input.
3. **Lore-consistent** — Would a Nova-Forge captain believe this system exists in-universe.

---

## Phase Overview

| Phase | Name | Key Systems | Status |
|-------|------|------------|--------|
| **1** | SwissAgent Core Expansion | media_pipeline, audio_pipeline, video_pipeline, stable_diffusion, zip_tools, installer_builder, qa_pipeline, versioning, dependency_manager, overlay modules | 📋 Planned |
| **2** | Nova-Forge ECS Expansion | VoxelBuildSystem, HoloPanelSystem, ClimateSimSystem, EcosystemSimSystem, CivSimSystem, GovernanceSystem, RagdollSystem, CrisisAISystem, SelfRepairAISystem | 📋 Planned |
| **3** | Engine Enhancement Stubs | RayTracing, GlobalIllumination, Volumetric, Scripting (Lua/Python), Serialization, LOD Streaming | 📋 Planned |
| **4** | Character & Rig Systems | IKFKBlending, FacialExpression, RagdollPhysics, HoloInterfacePanel full integration | 📋 Planned |
| **5** | Voxel World | VoxelWorld, VoxelTerrain, VoxelTools, Terraforming (expanded), VoxelPhysicsBlocks | 📋 Planned |
| **6** | Planetary Ecosystem | ClimateSim, EcosystemSim, procedural biomes, flora/fauna, ResourceNodeGen | 📋 Planned |
| **7** | SwissAgent IDE Integration | IDEBridge, IDEPanel, IDEWorkspace, IDEHotReload, IDEBuildManager, IDEAIController, Monaco, Open-WebUI | 📋 Planned |
| **8** | Blender Integration Addon | ProceduralGenerator, Exporter, PrefabLinker, AIIntegration Python add-on | 📋 Planned |
| **9** | Simulation Tiers | CivSim, Governance, Colony AI, BattleSim, CrisisAI, Diplomacy, Reputation | 📋 Planned |
| **10** | Platform & Studio Services | MultiUser, VersionControl, AssetLock, Tasks, BuildFarm, Accounts, Telemetry, Marketplace | 📋 Planned |

---

## Phase 1 — SwissAgent Core Expansion

**Goal:** Extend the existing `ai_dev/` SwissAgent platform with the full 34-module architecture described in the design document, including all media generation, packaging, and CI pipeline modules.

### Prerequisites
- Phase 0 (Seed Baseline) from [`docs/ROADMAP.md`](ROADMAP.md) ✅ Complete
- Ollama running locally with a coding model (DeepSeek Coder v2 / Qwen2.5 Coder)

### Checklist

#### 1.1 — Module Scaffold
- [ ] Create `ai_dev/modules/` directory with one sub-folder per module (34 total)
- [ ] Add `module.json` + `tools.json` + `src/` + `scripts/` skeleton to every module
- [ ] Implement `ModuleLoader` in `ai_dev/core/module_loader.py`
- [ ] Implement `ToolRegistry` in `ai_dev/core/tool_registry.py` — loads all `modules/*/tools.json`
- [ ] Write unit tests for `ModuleLoader` and `ToolRegistry`

#### 1.2 — Core Modules (must ship in Phase 1)
- [ ] **filesystem** — `read_file`, `write_file`, `list_dir`, `patch_file`, `search_code`
- [ ] **git** — `git_clone`, `git_commit`, `git_push`, `git_diff`, `github_pr`
- [ ] **zip** — `zip_pack`, `zip_extract`, `tar_pack`, `archive_folder`
- [ ] **build** — `cmake_configure`, `cmake_build`, `msbuild_run`, `parse_errors`
- [ ] **security** — `check_permission`, `sandbox_path`, `audit_access`; enforce `allowed_dirs` / `blocked_dirs`
- [ ] **memory** — `store_memory`, `recall_memory`, `search_memory` (replaces ad-hoc session dict)
- [ ] **cache** — `cache_get`, `cache_set`, `invalidate_cache` (LRU + disk)
- [ ] **index** — `find_symbol`, `find_class`, `find_ref`, `scan_project`

#### 1.3 — Media Pipeline Modules
- [ ] **image** module — integrate `StableDiffusionInterface` wrapper; stub for CPU-only fallback
- [ ] **audio** module — Bark TTS stub + AudioCraft MusicGen/AudioGen stub
- [ ] Create `ai_dev/tools/media_pipeline.py` implementing:
  - `generate_2d_texture(name, prompt, style, size)`
  - `generate_3d_model(name, description, format)`
  - `generate_audio(type, name, text, style)`
  - `generate_video(name, frames_source, fps, codec)`
  - `generate_shader_texture(name, glsl_template, resolution)`
- [ ] Register `media_pipeline` tools in `image` and `audio` modules

#### 1.4 — Pipeline & Packaging Modules
- [ ] **pipeline** module — task graph executor with `pipeline_run`, `pipeline_build`, `pipeline_test`, `pipeline_package`
- [ ] **installer** module — NSIS/Inno Setup wrapper stubs (`gen_installer`, `gen_updater`, `sign_binary`)
- [ ] **package** module — vcpkg/pip/npm wrappers (`install_package`, `update_deps`, `lock_deps`)
- [ ] **ci** module — GitHub Actions YAML generator (`gen_ci_yaml`, `run_local_ci`)
- [ ] **job** module — parallel job queue with dependency graph

#### 1.5 — Tooling & Dev Modules
- [ ] **test** module — `gen_test`, `run_tests`, `parse_coverage` integrated with `cpp_server/run_tests.sh`
- [ ] **doc** module — `gen_readme`, `gen_api_docs`, `extract_comments`
- [ ] **debug** module — `parse_log`, `parse_crash`, `inject_debug_print`
- [ ] **profile** module — `run_profiler`, `parse_profile`, `gen_perf_report`
- [ ] **template** module — `apply_template`, `list_templates`, `create_template`; ship ECS system template

#### 1.6 — Plugin System
- [ ] Implement `PluginLoader` in `ai_dev/core/plugin_loader.py` — loads `plugins/*/plugin.json`
- [ ] Create `plugins/stable_diffusion/` stub with `plugin.json` + `tools.json` + `sd_interface.py`
- [ ] Create `plugins/bark_tts/` stub
- [ ] Create `plugins/audiocraft/` stub
- [ ] Create `plugins/ollama_backend/` stub (wraps existing `LocalLLMBackend.cpp` via HTTP)

#### 1.7 — Workspace System
- [ ] Implement `WorkspaceManager` in `ai_dev/core/workspace_manager.py`
- [ ] `workspace.json` schema definition and validation
- [ ] Project-scoped configuration override system

#### 1.8 — Versioning
- [ ] Add `version` field to all `module.json` files
- [ ] Implement `VersionManager` — checks module compatibility, blocks incompatible combinations
- [ ] Add `CHANGELOG.md` to `ai_dev/`

#### 1.9 — Tests
- [ ] `ai_dev/tests/test_module_loader.py`
- [ ] `ai_dev/tests/test_tool_registry.py`
- [ ] `ai_dev/tests/test_media_pipeline.py`
- [ ] `ai_dev/tests/test_plugin_loader.py`
- [ ] `ai_dev/tests/test_workspace_manager.py`

---

## Phase 2 — Nova-Forge ECS Expansion

**Goal:** Implement nine new ECS systems in `cpp_server/` following the established [`SingleComponentSystem`](../cpp_server/include/ecs/single_component_system.h) pattern. Each system needs a component header, system header, implementation, and test file.

### Prerequisites
- Phase 1 complete (SwissAgent can scaffold the boilerplate)
- Baseline `cpp_server` builds cleanly: `cd cpp_server && mkdir -p build && cd build && cmake .. -DBUILD_TESTS=ON -DUSE_STEAM_SDK=OFF && cmake --build . -j$(nproc)`

### New Systems

| System | Component File | Header | Impl | Test |
|--------|---------------|--------|------|------|
| `HoloPanelSystem` | `character_components.h` | `holo_panel_system.h` | `holo_panel_system.cpp` | `test_holo_panel_system.cpp` |
| `VoxelBuildSystem` | `voxel_components.h` | `voxel_build_system.h` | `voxel_build_system.cpp` | `test_voxel_build_system.cpp` |
| `ClimateSimSystem` | `simulation_components.h` | `climate_sim_system.h` | `climate_sim_system.cpp` | `test_climate_sim_system.cpp` |
| `EcosystemSimSystem` | `simulation_components.h` | `ecosystem_sim_system.h` | `ecosystem_sim_system.cpp` | `test_ecosystem_sim_system.cpp` |
| `CivSimSystem` | `simulation_components.h` | `civ_sim_system.h` | `civ_sim_system.cpp` | `test_civ_sim_system.cpp` |
| `GovernanceSystem` | `simulation_components.h` | `governance_system.h` | `governance_system.cpp` | `test_governance_system.cpp` |
| `RagdollSystem` | `character_components.h` | `ragdoll_system.h` | `ragdoll_system.cpp` | `test_ragdoll_system.cpp` |
| `CrisisAISystem` | `ai_components.h` | `crisis_ai_system.h` | `crisis_ai_system.cpp` | `test_crisis_ai_system.cpp` |
| `SelfRepairAISystem` | `ai_components.h` | `self_repair_ai_system.h` | `self_repair_ai_system.cpp` | `test_self_repair_ai_system.cpp` |

### Checklist

#### 2.1 — New Component Headers
- [ ] Add `HoloPanelState` to `cpp_server/include/components/character_components.h`
  - Fields: `panel_visible`, `ai_active`, `panel_opacity`, `ai_message`, `active_zone`
- [ ] Create `cpp_server/include/components/voxel_components.h`
  - `VoxelChunkState` — chunk coords, dirty flag, voxel data pointer
  - `VoxelToolState` — active tool type, brush radius, material id
  - `TerraformOpState` — operation type, position, radius, strength
- [ ] Add to `cpp_server/include/components/simulation_components.h`:
  - `ClimateCellState` — temperature, pressure, humidity, wind vector
  - `EcosystemState` — biome type, flora density, fauna population, stress level
  - `CivState` — population, territory size, tech level, stability
  - `GovernanceState` — policy flags, tax rate, law enforcement strength
- [ ] Create `cpp_server/include/components/ai_components.h`
  - `CrisisState` — crisis type, severity, affected region, duration
  - `SelfRepairState` — target entity id, error description, repair attempts, last fix applied

#### 2.2 — HoloPanelSystem
- [ ] Header: `explicit HoloPanelSystem(ecs::World* world)` + `initialize`, `show`, `hide`, `set_zone`, `set_ai_message`, `is_visible`, `get_active_zone`
- [ ] Implementation: all mutating methods return `bool`; query methods return safe defaults
- [ ] Tests ≥ 50 assertions covering: init, show/hide, zone switching, AI message update, missing entity safety

#### 2.3 — VoxelBuildSystem
- [ ] Header: `initialize`, `place_block`, `remove_block`, `flood_fill`, `get_block`, `set_tool`
- [ ] Implementation: chunk dirty-marking on mutation; coordinate bounds validation
- [ ] Tests ≥ 50 assertions covering: place/remove round-trip, out-of-bounds safety, dirty flag propagation, missing entity

#### 2.4 — ClimateSimSystem
- [ ] Header: `initialize`, `set_temperature`, `set_humidity`, `set_wind`, `tick_climate`, `get_temperature`, `get_precipitation`
- [ ] Implementation: simple diffusion model per tick; clamp physical ranges
- [ ] Tests ≥ 50 assertions

#### 2.5 — EcosystemSimSystem
- [ ] Header: `initialize`, `set_biome`, `add_flora`, `add_fauna`, `apply_stress`, `get_flora_density`, `get_fauna_population`
- [ ] Implementation: predator/prey Lotka-Volterra update per tick
- [ ] Tests ≥ 50 assertions

#### 2.6 — CivSimSystem
- [ ] Header: `initialize`, `set_population`, `expand_territory`, `apply_tech_advance`, `apply_crisis`, `get_stability`, `get_population`
- [ ] Implementation: stability formula: `f(population, territory, tech, crisis_count)`
- [ ] Tests ≥ 50 assertions

#### 2.7 — GovernanceSystem
- [ ] Header: `initialize`, `set_tax_rate`, `set_law_enforcement`, `toggle_policy`, `get_tax_rate`, `get_approval_rating`
- [ ] Implementation: approval rating = `f(tax_rate, enforcement, active_policies)`
- [ ] Tests ≥ 50 assertions

#### 2.8 — RagdollSystem
- [ ] Header: `initialize`, `activate_ragdoll`, `deactivate_ragdoll`, `apply_impulse`, `is_active`, `get_root_velocity`
- [ ] Implementation: ragdoll state machine; secondary motion damping per tick
- [ ] Tests ≥ 50 assertions

#### 2.9 — CrisisAISystem
- [ ] Header: `initialize`, `spawn_crisis`, `escalate`, `resolve`, `get_severity`, `get_crisis_type`, `get_affected_region`
- [ ] Implementation: crisis lifecycle — `LATENT → ACTIVE → ESCALATING → RESOLVING → RESOLVED`
- [ ] Tests ≥ 50 assertions

#### 2.10 — SelfRepairAISystem
- [ ] Header: `initialize`, `report_error`, `attempt_repair`, `apply_fix`, `get_repair_attempts`, `is_repaired`
- [ ] Implementation: retry counter with exponential backoff; max attempts before escalation
- [ ] Tests ≥ 50 assertions

#### 2.11 — Registration
- [ ] Add all 9 `.cpp` files to `CORE_SOURCES` in `cpp_server/CMakeLists.txt`
- [ ] Add forward declarations and calls to `cpp_server/tests/test_main.cpp`
- [ ] Run full test suite: `cd cpp_server && ./run_tests.sh`
- [ ] Verify assertion count increases by ≥ 450 (9 systems × 50 minimum)

---

## Phase 3 — Engine Enhancement Stubs

**Goal:** Create stub implementations for advanced engine features in `engine/` (PascalCase conventions) so architecture is locked in before full implementation.

### Prerequisites
- Phase 2 complete
- Atlas Engine builds cleanly

### Checklist

#### 3.1 — Rendering Stubs (`engine/`)
- [ ] `RayTracing.h` / `RayTracing.cpp` — hardware RT shadow/reflection interface (DX12/Vulkan backend)
- [ ] `GlobalIllumination.h` / `GlobalIllumination.cpp` — voxel GI + screen-space probe interface
- [ ] `VolumetricClouds.h` / `VolumetricClouds.cpp` — planet-scale volumetric rendering stub
- [ ] `ShaderGraph.h` / `ShaderGraph.cpp` — node graph → GLSL transpiler interface
- [ ] `ProceduralSky.h` / `ProceduralSky.cpp` — day/night cycle, aurora, nebula backdrop

#### 3.2 — LOD Streaming (`engine/`)
- [ ] `LODManager.h` / `LODManager.cpp` — distance-based LOD selection, LOD level registration
- [ ] `AssetStreamer.h` / `AssetStreamer.cpp` — async background asset load/unload

#### 3.3 — Scripting (`engine/`)
- [ ] `LuaEngine.h` / `LuaEngine.cpp` — LuaJIT embed + Atlas API binding scaffold
  - `RegisterFunction()`, `CallFunction()`, `LoadScript()`, `ReloadScript()`
- [ ] `PythonEngine.h` / `PythonEngine.cpp` — pybind11 embed + Atlas API binding scaffold (optional build flag)

#### 3.4 — Serialization (`engine/`)
- [ ] `SaveManager.h` / `SaveManager.cpp` — incremental save with binary delta encoding
  - `Save(slot)`, `Load(slot)`, `DeleteSlot(slot)`, `ListSlots()`
- [ ] `VersionedSave.h` / `VersionedSave.cpp` — forward-compatible saves with migration callback registry

#### 3.5 — Audio Enhancements (`engine/`)
- [ ] `SpatialAudio.h` / `SpatialAudio.cpp` — HRTF + occlusion + Doppler extension of existing OpenAL system
- [ ] `ProceduralAudio.h` / `ProceduralAudio.cpp` — real-time SFX synthesis (wind, water, engine)
- [ ] `AdaptiveMusic.h` / `AdaptiveMusic.cpp` — game-state-driven music layer blending

#### 3.6 — Resource Virtualization (`engine/`)
- [ ] `VirtualFileSystem.h` / `VirtualFileSystem.cpp` — mount points for mods and DLC
- [ ] `ResourceVirtualization.h` / `ResourceVirtualization.cpp` — lazy loading + reference counting

#### 3.7 — Networking Extensions (`engine/`)
- [ ] `DistributedSim.h` / `DistributedSim.cpp` — simulation partitioning across server nodes
- [ ] `SyncManager.h` / `SyncManager.cpp` — client-side prediction + server reconciliation

#### 3.8 — CMake Integration
- [ ] Add all new engine `.cpp` files to `engine/CMakeLists.txt` under feature flags
  - `ENABLE_RAY_TRACING`, `ENABLE_LUA`, `ENABLE_PYTHON`, `ENABLE_VOXEL`
- [ ] Ensure clean build with `cmake .. -DBUILD_ENGINE=ON`

---

## Phase 4 — Character & Rig Systems

**Goal:** Fully implement character animation, inverse kinematics, facial expressions, ragdoll, and the Rig Holo-Interface Panel as playable systems.

### Prerequisites
- Phase 2 (`RagdollSystem`, `HoloPanelSystem` stubs complete)
- Phase 3 (LOD streaming + scripting stubs available)

### Checklist

#### 4.1 — IK / FK Blending (`cpp_server/`)
- [ ] Add `IKFKState` to `character_components.h` — blend weight, IK targets (hand/foot/eye), solver iterations
- [ ] Create `ikfk_blend_system.h` / `ikfk_blend_system.cpp`
  - `initialize`, `set_ik_target`, `set_blend_weight`, `solve_ik`, `get_effector_position`
- [ ] Create `test_ikfk_blend_system.cpp` ≥ 50 assertions

#### 4.2 — Facial Expression System (`cpp_server/`)
- [ ] Add `FacialState` to `character_components.h` — emotion enum, morph weights (joy, anger, fear, neutral), blink timer
- [ ] Create `facial_expression_system.h` / `facial_expression_system.cpp`
  - `initialize`, `set_emotion`, `blend_to_emotion`, `blink`, `get_morph_weight`
- [ ] Create `test_facial_expression_system.cpp` ≥ 50 assertions

#### 4.3 — Holo-Interface Panel (Full Integration)
- [ ] Expand `HoloPanelState` with: `gear_slots[8]`, `fleet_data`, `alert_list`, `comm_log[16]`
- [ ] Implement zone-specific data population methods in `HoloPanelSystem`:
  - `populate_gear_zone`, `populate_stats_zone`, `populate_fleet_zone`
  - `push_alert`, `push_comm_message`, `clear_alerts`
- [ ] Wire `HoloPanelSystem` to receive events from: `InventorySystem`, `HealthSystem`, `FleetSystem`, `AlertSystem`
- [ ] Client-side: atlas UI world-space panel rendering attached to forearm bone
- [ ] Tests: expand `test_holo_panel_system.cpp` to ≥ 80 assertions

#### 4.4 — Ragdoll (Full Integration)
- [ ] Connect `RagdollSystem` to `RigidBodySystem` for impulse propagation
- [ ] Implement secondary motion damping — limb angular velocity decay per tick
- [ ] Death/stun trigger: `CombatSystem` notifies `RagdollSystem` on `hp = 0`
- [ ] Tests: expand `test_ragdoll_system.cpp` to ≥ 80 assertions

#### 4.5 — AnimGraph Extension (`engine/`)
- [ ] Extend `AnimGraph` with `IKFKBlendNode` and `RagdollBlendNode`
- [ ] Hot-reload support for AnimGraph JSON definitions via `IDEHotReload`

---

## Phase 5 — Voxel World

**Goal:** Implement the full voxel world system — chunk management, procedural terrain generation, physics-enabled blocks, and terraforming — as a server-simulated ECS system backed by an engine-side renderer.

### Prerequisites
- Phase 2 (`VoxelBuildSystem` stub complete)
- Phase 3 (`ENABLE_VOXEL` engine flag scaffolded)

### Checklist

#### 5.1 — Voxel Engine Layer (`engine/`)
- [ ] `VoxelWorld.h` / `VoxelWorld.cpp` — chunk manager, storage (16³ chunks), load/unload, serialization
- [ ] `VoxelTerrain.h` / `VoxelTerrain.cpp` — procedural terrain from layered noise + biome rules
- [ ] `VoxelMeshOptimizer.h` / `VoxelMeshOptimizer.cpp` — greedy meshing, LOD-based simplification
- [ ] `VoxelLightPropagation.h` / `VoxelLightPropagation.cpp` — flood-fill light inside structures
- [ ] `VoxelPhysicsBlocks.h` / `VoxelPhysicsBlocks.cpp` — fluid, gas, lava block types with flow simulation

#### 5.2 — Voxel Server Systems (`cpp_server/`)
- [ ] Expand `VoxelBuildSystem` beyond stub:
  - Chunk dirty-flag propagation across chunk borders
  - Structural integrity check on block removal (breadth-first from root)
  - Material property lookup: weight, conductivity, hardness per voxel type
- [ ] Create `VoxelMiningSystem` — mining progress per block, yield calculation, ore spawning
  - `start_mine`, `tick_mine`, `finish_mine`, `get_progress`, `get_yield`
- [ ] Create `VoxelTerraformSystem` — operator-driven terrain sculpting
  - `raise_terrain`, `lower_terrain`, `smooth_terrain`, `paint_surface`
- [ ] Tests for all voxel server systems ≥ 50 assertions each

#### 5.3 — Voxel Tools Integration (`editor/`)
- [ ] `VoxelToolsPanel` — brush size, material picker, mode (place/remove/paint/fill)
- [ ] Real-time chunk re-mesh on dirty flag in editor viewport

#### 5.4 — Content
- [ ] Define `voxel_materials.json` — 64 base materials (rock, ore types, ice, metal, soil, flora)
- [ ] Define `voxel_biome_rules.json` — surface/subsurface layer rules per biome type

---

## Phase 6 — Planetary Ecosystem

**Goal:** Implement planet-scale climate simulation, procedural biome generation, ecosystem life cycles, and resource node distribution.

### Prerequisites
- Phase 5 (Voxel terrain provides the surface to populate)
- Phase 2 (`ClimateSimSystem`, `EcosystemSimSystem` stubs complete)

### Checklist

#### 6.1 — Climate Simulation (`cpp_server/`)
- [ ] Expand `ClimateSimSystem` to full implementation:
  - Spherical temperature gradient (equator hot → poles cold)
  - Pressure cells driving wind patterns
  - Humidity advection, precipitation, cloud cover
  - Seasonal variation (axial tilt parameter)
  - `get_precipitation`, `get_cloud_cover`, `get_seasonal_modifier`
- [ ] Tests: expand to ≥ 80 assertions

#### 6.2 — Ecosystem Simulation (`cpp_server/`)
- [ ] Expand `EcosystemSimSystem` to full implementation:
  - Biome assignment from climate cell data (temperature + humidity → biome type)
  - Flora density from soil fertility + rainfall
  - Fauna population from flora density (prey) + predator count
  - Migration: fauna moves between adjacent ecosystem entities when stressed
  - `apply_pollution`, `apply_fire`, `recover_tick`
- [ ] Tests: expand to ≥ 80 assertions

#### 6.3 — Planet Generator (`engine/`)
- [ ] `PlanetGenerator.h` / `PlanetGenerator.cpp`:
  - Sphere UV unwrapping to latitude/longitude grid
  - Noise-based heightmap → biome classification → voxel terrain seed
  - `GeneratePlanet(seed, radius, axial_tilt, water_coverage)` → planet asset

#### 6.4 — Resource Node Generation (`cpp_server/`)
- [ ] Expand `ResourceNodeGen` to place nodes based on biome + subsurface geology:
  - Surface: scattered outcroppings (small yield, easy access)
  - Subsurface: massive deposits (high yield, requires drilling/tunnelling)
  - Random trace minerals in soil (passive yield when building)
- [ ] JSON config: `resource_biome_weights.json` — per-biome ore probability tables

#### 6.5 — Orbit Physics (`cpp_server/`)
- [ ] Create `OrbitPhysicsSystem` — Keplerian two-body orbit with perturbation support
  - `initialize_orbit`, `tick_orbit`, `get_position`, `get_velocity`, `get_orbital_period`
  - Emergent events: elliptical decay, gravitational slingshot
- [ ] Tests ≥ 50 assertions

#### 6.6 — Flora/Fauna Procedural Content
- [ ] `procedural_flora.json` — 32 plant archetypes with biome affinity, growth rate, resource yield
- [ ] `procedural_fauna.json` — 16 creature archetypes with diet, territory, loot table
- [ ] `EcosystemSpawner` — places flora/fauna entities on server startup from `EcosystemState`

---

## Phase 7 — SwissAgent IDE Integration

**Goal:** Surface the full SwissAgent agent loop and tool system inside the Atlas Editor via an embedded Monaco editor panel and Open-WebUI chat, bridged to the C++ engine via `IDEBridge`.

### Prerequisites
- Phase 1 (SwissAgent modules operational)
- Atlas Editor with at least one available docking slot

### Checklist

#### 7.1 — IDEBridge (`editor/ai/`)
- [ ] `IDEBridge.h` / `IDEBridge.cpp` — named-pipe + JSON-RPC IPC between C++ editor and Python SwissAgent
  - `SendPrompt(prompt)` → async response via callback
  - `ExecuteTool(tool_name, args_json)` → result JSON
  - `GetAgentStatus()` → running / idle / error
- [ ] Python counterpart: `ai_dev/core/ide_bridge_server.py` — listens on named pipe / local socket

#### 7.2 — IDEPanel (`editor/panels/`)
- [ ] `IDEPanel.h` / `IDEPanel.cpp` — Atlas UI docking panel hosting:
  - Monaco editor web-view (via embedded Chromium or CEF if available, otherwise custom text editor)
  - Open-WebUI chat pane (HTTP to local Open-WebUI server)
  - Build log output area
  - Agent status indicator

#### 7.3 — IDEWorkspace (`editor/ai/`)
- [ ] `IDEWorkspace.h` / `IDEWorkspace.cpp` — syncs `workspace.json` with Atlas project system
  - On project open: writes `workspace.json` with current source tree
  - On file save: notifies `IDEBridge` for incremental index update

#### 7.4 — IDEHotReload (`editor/ai/`)
- [ ] `IDEHotReload.h` / `IDEHotReload.cpp` — file watcher that triggers reload per file type:
  - `.glsl` → shader recompile + hot-swap
  - `.lua` → LuaEngine `ReloadScript()`
  - `.json` → data re-parse + `DataReloadEvent` broadcast
  - `.cpp` / `.h` → incremental CMake build via `IDEBuildManager`

#### 7.5 — IDEBuildManager (`editor/ai/`)
- [ ] `IDEBuildManager.h` / `IDEBuildManager.cpp` — wraps `cmake --build` invocation
  - Streams compiler output lines to `IDEPanel` build log in real time
  - Parses GCC/Clang/MSVC errors; sends structured error list to `IDEAIController`

#### 7.6 — IDEAIController (`editor/ai/`)
- [ ] `IDEAIController.h` / `IDEAIController.cpp` — receives AI suggestion diffs from `IDEBridge`
  - Presents diff in `IDEPanel` with Accept / Reject / Partial buttons
  - On Accept: applies changes via `EditorCommandBus` (undo-able)

#### 7.7 — SwissAgentPanel (`editor/panels/`)
- [ ] `SwissAgentPanel.h` / `SwissAgentPanel.cpp` — dedicated agent control panel:
  - Model selection dropdown (lists models from `ai_dev/models/`)
  - Active module toggles
  - Workspace selector
  - Live tool call log

#### 7.8 — Overlay Activation
- [ ] Wire `Alt+F12` keybind to toggle `IDEPanel` visibility in editor
- [ ] FPS playtest mode: same keybind surfaces the `SwissAgentPanel` HUD overlay

---

## Phase 8 — Blender Integration Addon

**Goal:** Deliver a Blender Python add-on that provides Nova-Forge asset generation, export, and prefab-linking workflows directly from Blender's UI.

### Prerequisites
- Phase 1 (SwissAgent `blender` module and `image` module operational)
- Phase 7 (SwissAgent HTTP API endpoint for prompt→script generation)

### Checklist

#### 8.1 — Add-on Registration
- [ ] Create `blender_addon/__init__.py` — add-on metadata, panel registration, operator registration
- [ ] `bl_info` metadata: name = "Nova-Forge Asset Bridge", category = "Import-Export"
- [ ] Preferences panel: SwissAgent API URL, export output directory, default LOD count

#### 8.2 — ProceduralGenerator (`blender_addon/ProceduralGenerator.py`)
- [ ] `NFProceduralGeneratorPanel` — side panel in 3D Viewport > Nova-Forge tab
- [ ] `NFGenerateMeshOperator` — prompt input → calls SwissAgent `/api/tools/blender_gen_model` → executes returned Blender Python script
- [ ] `NFGenerateTextureOperator` — prompt → StableDiffusion → assigns to active material
- [ ] Geometry nodes templates for: ship hull, asteroid, terrain feature, space station module

#### 8.3 — Exporter (`blender_addon/Exporter.py`)
- [ ] `NFExportOperator` — batch FBX/GLB export with:
  - Automatic LOD mesh generation (4 levels: full, 75%, 50%, 25% vertex count)
  - Material baking: albedo, normal, roughness/metallic → texture atlases
  - Asset metadata embedding: `nova_forge_asset.json` alongside exported file
  - Naming convention enforcement: `snake_case` for Nova-Forge assets

#### 8.4 — PrefabLinker (`blender_addon/PrefabLinker.py`)
- [ ] `NFPrefabLinkPanel` — UI panel showing linked prefab JSON path
- [ ] `NFLinkToPrefabOperator` — reads `nova_forge_asset.json`, opens file picker, writes link
- [ ] `NFSyncFromPrefabOperator` — detects transform/material divergence, reports delta
- [ ] `NFPushToPrefabOperator` — writes Blender scene state back to prefab JSON

#### 8.5 — AIIntegration (`blender_addon/AIIntegration.py`)
- [ ] `SwissAgentAPIClient` — HTTP client pointing to local SwissAgent server
  - `send_prompt(prompt)` → structured response
  - `call_tool(name, args)` → tool result
- [ ] `NFAIAssistantPanel` — chat-style prompt input inside Blender
- [ ] `NFAIGenerateScriptOperator` — returns a Blender Python script, shows diff, user approves, script executes

#### 8.6 — Testing
- [ ] Manual test: generate asteroid model from prompt, export LODs, link to prefab JSON
- [ ] Automated: `blender --background --python blender_addon/tests/test_exporter.py`

---

## Phase 9 — Simulation Tiers

**Goal:** Implement the full Simulation Tier ECS systems — civilisation, governance, colony, fleet battle, crisis AI, diplomacy, and reputation — all running on the server tick.

### Prerequisites
- Phase 2 (`CivSimSystem`, `GovernanceSystem`, `CrisisAISystem` stubs)
- Phase 6 (Planetary ecosystem provides the terrain for colony placement)

### Checklist

#### 9.1 — Diplomacy System (`cpp_server/`)
- [ ] Add `DiplomacyState` to `simulation_components.h` — relations table, active treaties, pending proposals
- [ ] Create `diplomacy_system.h` / `diplomacy_system.cpp`
  - `initialize`, `propose_treaty`, `accept_treaty`, `break_treaty`, `declare_war`, `negotiate_peace`
  - `get_relation_score(faction_a, faction_b)` → int [-100, 100]
- [ ] Tests ≥ 50 assertions

#### 9.2 — Reputation System (`cpp_server/`)
- [ ] Add `ReputationState` to `simulation_components.h` — per-faction standing, crime flag, bounty amount
- [ ] Create `reputation_system.h` / `reputation_system.cpp`
  - `initialize`, `apply_action`, `decay_reputation`, `get_standing`, `is_wanted`, `get_bounty`
- [ ] Tests ≥ 50 assertions

#### 9.3 — Colony AI System (`cpp_server/`)
- [ ] Add `ColonyState` to `simulation_components.h` — population, morale, resource stockpiles[16], buildings[32]
- [ ] Create `colony_system.h` / `colony_system.cpp`
  - `initialize`, `add_building`, `remove_building`, `set_resource`, `consume_resources`, `get_morale`
- [ ] Create `building_ai_system.h` / `building_ai_system.cpp` — NPC construction scheduling
- [ ] Tests ≥ 50 assertions each

#### 9.4 — Battle Simulation (`cpp_server/`)
- [ ] Expand existing combat systems with `BattleSimSystem`:
  - `initialize_battle(fleet_a, fleet_b)`, `tick_battle`, `get_outcome`, `get_casualties`
  - Uses existing `FleetAI` and `TacticsAI` component data
- [ ] Tests ≥ 50 assertions

#### 9.5 — CivSim Expansion
- [ ] Expand `CivSimSystem` beyond stub:
  - Lifecycle phases: `FOUNDING → GROWTH → PEAK → STAGNATION → COLLAPSE`
  - Tech tree: 8 levels, each unlocking new building types
  - Cultural trait system: 4 axes (militarism, commerce, science, culture)
  - Integration with `DiplomacySystem` and `EconomyAI`
- [ ] Tests: expand to ≥ 80 assertions

#### 9.6 — Governance Expansion
- [ ] Expand `GovernanceSystem` beyond stub:
  - Government types: Democracy, Oligarchy, Dictatorship, Anarchy (transition rules)
  - Policy tree: 16 toggleable policies affecting tax, enforcement, freedom
  - Corruption mechanic: erodes effective policy implementation
  - Integration with `CivSimSystem` stability feedback loop
- [ ] Tests: expand to ≥ 80 assertions

#### 9.7 — CrisisAI Expansion
- [ ] Expand `CrisisAISystem` beyond stub:
  - 8 crisis types: plague, famine, civil war, natural disaster, economic collapse, invasion, piracy surge, titan emergence
  - Propagation: crisis can spread to adjacent regions via `spread_to`
  - Player notification: pushes crisis alerts to `HoloPanelSystem`
- [ ] Tests: expand to ≥ 80 assertions

#### 9.8 — RTS Territory System (`cpp_server/`)
- [ ] Add `TerritoryState` to `simulation_components.h` — hex grid control map, border hex list, contested hexes
- [ ] Create `territory_system.h` / `territory_system.cpp`
  - `initialize`, `claim_hex`, `contest_hex`, `resolve_borders`, `get_controlled_area`
- [ ] Tests ≥ 50 assertions

#### 9.9 — Registration
- [ ] Add all new Phase 9 `.cpp` files to `cpp_server/CMakeLists.txt`
- [ ] Register all tests in `test_main.cpp`
- [ ] Run full suite: `cd cpp_server && ./run_tests.sh`

---

## Phase 10 — Platform & Studio Services

**Goal:** Implement multi-user collaborative editing, version control integration, asset locking, build farm, accounts, telemetry, and marketplace as editor and platform services.

### Prerequisites
- Phase 7 (IDE integration provides the foundation for multi-user sync)

### Checklist

#### 10.1 — Multi-User Editing (`editor/`)
- [ ] `MultiUserSession.h` / `MultiUserSession.cpp` — collaborative session management
  - `CreateSession`, `JoinSession`, `LeaveSession`, `BroadcastEdit`
- [ ] `SyncManager.h` / `SyncManager.cpp` — operational transform for simultaneous edits
  - `ApplyOp`, `TransformOp`, `AcknowledgeOp`
- [ ] `ConflictResolver.h` / `ConflictResolver.cpp` — three-way merge for ECS component data
  - `DetectConflict`, `ResolveConflict`, `ShowMergeDiff`
- [ ] `MergeAssistantPanel` — shows conflict diffs with AI-suggested resolution

#### 10.2 — Version Control (`editor/`)
- [ ] `GitClient.h` / `GitClient.cpp` — embedded Git operations via libgit2 or subprocess
  - `Status`, `Stage`, `Commit`, `Push`, `Pull`, `CreateBranch`, `Merge`
- [ ] `GitPanel` — shows staged changes, branch list, commit history inside editor
- [ ] Wire to `SwissAgent` `git` module for AI-assisted commit message generation

#### 10.3 — Asset Lock (`editor/`)
- [ ] `AssetLock.h` / `AssetLock.cpp` — pessimistic lock system
  - `AcquireLock(asset_path, user)`, `ReleaseLock`, `IsLocked`, `GetLockOwner`
  - Lock lease timeout: 30 minutes; auto-release on session disconnect
- [ ] Visual indicator in `AssetBrowser` showing locked assets

#### 10.4 — Task Board (`editor/`)
- [ ] `TaskBoard.h` / `TaskBoard.cpp` — Kanban task data model (Backlog / In Progress / Review / Done)
  - `CreateTask`, `MoveTask`, `AssignTask`, `CloseTask`
- [ ] `TaskBoardPanel` — Kanban UI inside editor
- [ ] Optional: sync task status with GitHub Issues via `git` module

#### 10.5 — Build Farm (`editor/`)
- [ ] `BuildFarm.h` / `BuildFarm.cpp` — distributed build job submission
  - `SubmitJob(config)`, `GetJobStatus(id)`, `CancelJob(id)`, `GetArtifacts(id)`
- [ ] `BuildFarmPanel` — shows queued/running/completed jobs and download links

#### 10.6 — Accounts & Auth (`cpp_server/`)
- [ ] Add `AccountState` + `AuthState` + `ProfileState` to platform components header
- [ ] Create `account_system.h` / `account_system.cpp` — register, login, logout
- [ ] Create `auth_system.h` / `auth_system.cpp` — session token generation, bcrypt password hashing
- [ ] Tests ≥ 50 assertions each

#### 10.7 — Telemetry (`cpp_server/`)
- [ ] Create `telemetry_system.h` / `telemetry_system.cpp` — opt-in session analytics
  - `record_event(name, data_json)`, `flush_batch`, `is_enabled`
- [ ] All data anonymised; no PII; stored locally and optionally pushed to self-hosted endpoint
- [ ] Tests ≥ 50 assertions

#### 10.8 — Marketplace (`cpp_server/` + `editor/`)
- [ ] `MarketplaceClient.h` / `MarketplaceClient.cpp` — REST client for Nova-Forge asset marketplace
  - `Browse`, `Search`, `Download`, `Upload`, `GetListingDetails`
- [ ] `MarketplacePanel` — browsable grid of community assets inside editor
- [ ] Wire to `ModSDK` for mod validation before install

#### 10.9 — Registration
- [ ] Add all Platform Tier `.cpp` files to `cpp_server/CMakeLists.txt`
- [ ] Register tests in `test_main.cpp`
- [ ] Run full suite; verify passing assertion count

---

## Assertion Count Targets

| Phase | New Systems | Min Assertions Added | Cumulative Target |
|-------|-------------|---------------------|-------------------|
| Baseline | — | — | 18 747+ |
| **Phase 2** | 9 systems | +450 | 19 200+ |
| **Phase 4** | 2 new + 2 expanded | +320 | 19 520+ |
| **Phase 5** | 3 voxel systems | +150 | 19 670+ |
| **Phase 6** | OrbitPhysics + expansions | +260 | 19 930+ |
| **Phase 9** | 6 new + 3 expanded | +560 | 20 490+ |
| **Phase 10** | Accounts + Telemetry | +100 | 20 590+ |

---

## Quick Reference — File Naming Conventions

| Location | Headers | Implementations | Tests |
|----------|---------|-----------------|-------|
| `cpp_server/` | `snake_case.h` | `snake_case.cpp` | `test_snake_case.cpp` |
| `engine/` | `PascalCase.h` | `PascalCase.cpp` | — |
| `editor/` | `PascalCase.h` | `PascalCase.cpp` | — |
| `ai_dev/` | — | `snake_case.py` | `test_snake_case.py` |
| `blender_addon/` | — | `PascalCase.py` | `test_*.py` |

---

## Related Documents

| Document | Contents |
|----------|---------|
| [`docs/design/NEW_IMPLEMENTATIONS_DESIGN.md`](design/NEW_IMPLEMENTATIONS_DESIGN.md) | Deep design detail for every system in this roadmap |
| [`docs/ROADMAP.md`](ROADMAP.md) | Existing systems (18 747+ assertions), vertical slice, Dev AI Phase 0 |
| [`docs/design/NOVADEV_AI.md`](design/NOVADEV_AI.md) | NovaForge Dev AI offline agent design |
| [`docs/design/MASTER_DESIGN_BIBLE.md`](design/MASTER_DESIGN_BIBLE.md) | Game design vision and universe lore |
| [`docs/archive/NEXT_TASKS.md`](archive/NEXT_TASKS.md) | Running log of implemented systems and assertion counts |

---

*Last updated: extracted and structured from `new implementations.md` (31 696-line design archive).*

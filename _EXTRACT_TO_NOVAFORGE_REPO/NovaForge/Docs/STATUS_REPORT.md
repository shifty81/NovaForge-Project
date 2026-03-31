# NovaForge — Comprehensive Project Status Report

> **Generated:** March 24, 2026  
> For the roadmap, see [`docs/ROADMAP.md`](ROADMAP.md).  
> For EVE feature completeness, see [`docs/design/EVE_FEATURE_GAP.md`](design/EVE_FEATURE_GAP.md).  
> For deep design detail, see [`docs/design/MASTER_DESIGN_BIBLE.md`](design/MASTER_DESIGN_BIBLE.md).

---

## Table of Contents

1. [Project Overview](#1-project-overview)
2. [Server ECS — Systems & Tests](#2-server-ecs--systems--tests)
3. [Atlas Engine](#3-atlas-engine)
4. [Client (cpp_client)](#4-client-cpp_client)
5. [Editor Tool Layer](#5-editor-tool-layer)
6. [NovaForge Dev AI](#6-novaforge-dev-ai)
7. [Blender PCG Pipeline](#7-blender-pcg-pipeline)
8. [Design Phases A–G Status](#8-design-phases-ag-status)
9. [EVE Feature Gap Summary](#9-eve-feature-gap-summary)
10. [Documentation & Data](#10-documentation--data)
11. [Gaps Summary — What's Missing](#11-gaps-summary--whats-missing)
12. [Recommended Next Steps](#12-recommended-next-steps)

---

## 1. Project Overview

**Nova Forge** is a PvE-focused space simulator for solo or small groups (2–20 players), built on the custom **Atlas Engine** (C++17/OpenGL). The game delivers EVE-like depth — ships, skills, fitting, combat, missions, exploration — powered by an AI-driven economy where every NPC is a real economic actor.

| Property | Value |
|----------|-------|
| Engine | Atlas Engine (custom C++/OpenGL) |
| Architecture | Server-authoritative, tick-based (10–20 Hz), deterministic |
| Scope | PvE, solo + small group (2–20 players) |
| Active Branch | Vertical Slice Integration + Client Polish |
| Current Phase | Tooling Layer + Dev Solar System → Vertical Slice |

### Repository Layout

```
NovaForge/
├── cpp_server/     ← Game server: ECS, systems, simulation, tests
├── cpp_client/     ← OpenGL game client: rendering, HUD, input
├── engine/         ← Atlas Engine: ECS core, rendering, audio, PCG
├── editor/         ← Atlas Editor GUI
├── ai_dev/         ← NovaForge Dev AI: offline LLM agent loop
├── tools/          ← Blender spaceship generator + utilities
├── data/           ← JSON game content (ships, modules, skills, etc.)
├── docs/           ← 211 markdown documents across 21 subdirectories
└── schemas/        ← JSON schema validation
```

---

## 2. Server ECS — Systems & Tests

### ✅ Fully Complete

| Metric | Value |
|--------|-------|
| **ECS System Headers** | 452 |
| **ECS System Implementations (.cpp)** | 452 (100%) |
| **ECS System Test Files** | 452+ (100%) |
| **Total Test Assertions** | ~24,000+ |
| **Missing implementations** | **0** |
| **Missing test files** | **0** |

Every system header in `cpp_server/include/systems/` has a matching `.cpp` implementation in `cpp_server/src/systems/` and a test file in `cpp_server/tests/`.

### Systems by Domain

| Domain | Components File | Key Systems |
|--------|----------------|-------------|
| **Combat** | `combat_components.h` | WeaponSystem, TargetingSystem, CapacitorSystem, ShieldRechargeSystem, ECMJammingSystem, RemoteRepairSystem, WarpScramblerSystem, FighterSquadronSystem |
| **Ships & Modules** | `ship_components.h` | ModuleSystem, FittingValidationSystem, Tech2ModuleSystem, PropulsionModuleSystem, InertiaModifierSystem, TrackingDisruptionSystem |
| **Navigation** | `navigation_components.h` | MovementSystem, WormholeSystem, JumpCloneSystem, DirectionalScanSystem, SurveyScannerSystem, SimSensorConfidenceSystem |
| **Economy** | `economy_components.h` | MarketSystem, WalletJournalSystem, CorpTaxLedgerSystem, ContractSystem, MoonMiningSchedulerSystem, CargoContainerSystem, FuelBlockSystem |
| **Fleet & Social** | `fleet_components.h`, `social_components.h` | FleetSystem, FleetDepartureSystem, FleetCultureSystem, CaptainMentorshipSystem, CaptainAmbitionSystem, CaptainStressSystem, EmotionalArcSystem, FactionDoctrineSystem |
| **Exploration** | `exploration_components.h` | AnomalySystem, WormholeSystem, SpaceScarSystem, SystemEventSystem, AmbientEventSystem, SectorTensionSystem, AbyssalFilamentSystem |
| **Game & Progression** | `game_components.h` | SkillSystem, MissionSystem, BountySystem, InsuranceSystem, CharacterSheetSystem, JumpCloneSystem |
| **UI** | `ui_components.h` | OverviewSystem, NotificationSystem, HangarSystem, BookmarkSystem, FleetAdvertisementSystem |
| **NPC & AI** | `npc_components.h` | NpcDatabase, AmbientTrafficSystem, NpcIntentSystem, PlayerPresenceSystem |

### Build Commands

```bash
# Build all server tests
cd cpp_server
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DUSE_STEAM_SDK=OFF
cmake --build . -j$(nproc)

# Run all server tests
./bin/test_systems

# Build + run a single system's tests
cmake --build . --target test_<name>_system -j$(nproc)
./bin/test_<name>_system
```

---

## 3. Atlas Engine

### ✅ Complete

| Module | Status | Description |
|--------|--------|-------------|
| `core/` | ✅ | Engine bootstrap, EventBus, Logger, GameStateManager, RuntimeBootstrap |
| `ecs/` | ✅ | ECS world, entities, SingleComponentSystem base, DeltaEditStore |
| `rendering/` (OpenGL) | ✅ | Deferred pipeline, shadow mapping, post-processing, instanced rendering |
| `audio/` | ✅ | OpenAL positional 3D audio, fleet audio, warp adaptive audio |
| `input/` | ✅ | InputManager |
| `camera/` | ✅ | Camera, CameraProjectionPolicy |
| `animation/` | ✅ | AnimationGraph, clip/blend/modifier/state machine nodes |
| `net/` | ✅ | TCP client/server, delta compression, jitter buffer, lag compensation, NetworkQualityMonitor |
| `physics/` | ✅ | Rigid body, AABB collision |
| `plugin/` | ✅ | PluginSystem |
| `graphvm/` | ✅ | GraphVM, GraphIR, GraphCompiler |
| `world/` | ✅ | CubeSphereLayout, VoxelGridLayout, WorldLayout |
| `sim/` | ✅ | TickScheduler, AI state machines (Miner/Hauler/Pirate/Security/Trader), EconomyRulesLoader, UniverseMapSystem, AdvancedMissionGenerator |
| `assets/` | ✅ | AssetRegistry, binary loading, serialization |

**Atlas Engine Tests:** 915 passing assertions (input, camera, audio, animation, plugin).

**Atlas UI:** Full retained-mode widget system — widgets, panels, docking (DockNode tree), Photon Dark sci-fi theme.

**Atlas Editor:** 21 panels, PCG tools, dock layout, debug console.

---

## 4. Client (cpp_client)

### Mostly Complete

| Area | Status | Notes |
|------|--------|-------|
| **OpenGL Rendering** | ✅ | Deferred pipeline, shadow maps, post-processing, instanced meshes |
| **Ship HUD** | ✅ | Control ring (shield/armor/hull arcs), capacitor bar, module rack, target brackets |
| **FPS & Interior Mode** | ✅ | EVA airlock, salvage paths, clone bay |
| **PCG Ship Generation** | ✅ | Spine-based hulls, turrets, faction shape language, procedural materials |
| **Audio** | ✅ | OpenAL 3D, warp adaptive audio, fleet positional audio |
| **Camera** | ✅ | Orbit + FPS modes, correct RHS convention (-Z forward at yaw=0) |
| **Solar System Scene** | ✅ Partial | "Asakai" test system: Sun + 3 planets + 2 asteroid belts — needs expansion |
| **Networking** | ✅ | Snapshot replication, interpolation, jitter buffer |
| **Atlas UI in Client** | ✅ | All game panels use Atlas UI docking + EVE-style dark theme |
| **Client–Server Wiring** | 🔧 In Progress | Not all 452 server systems are wired to client rendering/UI |
| **Performance Profiling** | 🔧 In Progress | Target: 20 Hz server with 500+ entities; 60 FPS client with 200 ships |

**Client test coverage:** 21 test files covering entity sync, frustum culling, instanced rendering, audio, lighting, shadow mapping, network, physics, deferred rendering, procedural meshes, texture loading, post-processing.

**Outstanding TODOs in cpp_client (4):**
- `// TODO: multiplayer lobby / server browser`
- `// TODO: dispatch interaction to nearest interactable entity`
- `// TODO: Could show a general error dialog here`
- `// TODO: Use these values for interpolation delay calculation`

---

## 5. Editor Tool Layer

### 🔧 In Progress — Biggest Current Gap

The Editor Tool Layer is the most significant outstanding gap. Of 32 planned editor tools, only 5 have working `.cpp` implementations.

### Implemented (5 / 32)

| Tool | Header | Implementation |
|------|--------|---------------|
| `EditorCommandBus` | `editor_command_bus.h` | ✅ `editor_command_bus.cpp` |
| `EditorEventBus` | `editor_event_bus.h` | ✅ `editor_event_bus.cpp` |
| `EditorToolLayer` | `editor_tool_layer.h` | ✅ `editor_tool_layer.cpp` |
| `SceneBookmarkManager` | `scene_bookmark_manager.h` | ✅ `scene_bookmark_manager.cpp` |
| `UndoableCommandBus` | `undoable_command_bus.h` | ✅ `undoable_command_bus.cpp` |

### Header-Only — Need Implementation (27 / 32)

| Priority | Tool | Header | Purpose |
|----------|------|--------|---------|
| **P0** | `MultiSelectionManager` | `multi_selection_manager.h` | Select/group multiple scene assets |
| **P0** | `SnapAlignTool` | `snap_align_tool.h` | Grid/surface/asset snapping and alignment |
| **P0** | `PrefabLibrary` | `prefab_library.h` | Drag-and-drop reusable module library |
| **P0** | `CameraViewTool` | `camera_view_tool.h` | Free-fly, orbit, ortho camera views |
| **P0** | `LiveEditMode` | `live_edit_mode.h` | Edit scene entities while simulation is running |
| **P1** | `AnimationEditorTool` | `animation_editor_tool.h` | Multi-layer animation clip editing |
| **P1** | `IKRigTool` | `ik_rig_tool.h` | Inverse kinematics for characters and turrets |
| **P1** | `SimulationStepController` | `simulation_step_controller.h` | Pause, step, and scrub the simulation |
| **P1** | `EnvironmentControlTool` | `environment_control_tool.h` | Gravity, wind, atmosphere settings |
| **P1** | `MaterialShaderTool` | `material_shader_tool.h` | Live material and shader parameter editing |
| **P1** | `LightingControlTool` | `lighting_control_tool.h` | Light property editing and scene presets |
| **P2** | `FunctionAssignmentTool` | `function_assignment_tool.h` | Assign trigger functions to scene entities |
| **P2** | `EventTimelineTool` | `event_timeline_tool.h` | Scripted cutscene and sequence editor |
| **P2** | `NPCSpawnerTool` | `npc_spawner_tool.h` | AI/PCG asset spawning in the scene |
| **P2** | `MapEditorTool` | `map_editor_tool.h` | Star system map layout editing |
| **P2** | `ShipModuleEditorTool` | `ship_module_editor_tool.h` | Ship module slot visual editing |
| **P2** | `ResourceBalancerTool` | `resource_balancer_tool.h` | Visual resource distribution tool |
| **P3** | `PCGSnapshotManager` | `pcg_snapshot_manager.h` | Snapshot and rollback PCG state |
| **P3** | `DeltaEditsMergeTool` | `delta_edits_merge_tool.h` | Merge and resolve DeltaEdit conflicts |
| **P3** | `EditPropagationTool` | `edit_propagation_tool.h` | Propagate edits to similar asset instances |
| **P3** | `VisualDiffTool` | `visual_diff_tool.h` | Compare current scene vs PCG baseline |
| **P3** | `LayerTagSystem` | `layer_tag_system.h` | Asset categorization and layer visibility |
| **P3** | `AssetStatsPanel` | `asset_stats_panel.h` | Hierarchy, physics, memory stats panel |
| **P3** | `HotkeyActionManager` | `hotkey_action_manager.h` | Custom keybind assignment |
| **P3** | `ScriptConsole` | `script_console.h` | Real-time in-game debug console |
| **P3** | `BatchOperationsTool` | `batch_operations_tool.h` | Mass transformations across multiple assets |
| — | `ITool` (base) | `itool.h` | Base interface — header-only by design ✅ |

---

## 6. NovaForge Dev AI

An offline AI development studio built into the project — a local LLM agent that generates code, compiles, hot-reloads, and iterates, all from inside the engine.

### Phase Status

| Phase | Goal | Status |
|-------|------|--------|
| **0 — Seed Baseline** | Agent loop, local LLM, file ops, CMake runner, error parser | ✅ **COMPLETE** |
| **1 — Hot Reload** | File watcher, Lua/Python/GLSL/JSON selective reload, git ops, ECS scaffold, CLI mode | ✅ **COMPLETE** |
| **2 — Multi-Language** | IPC bridge (TCP 127.0.0.1:19850), C#/.NET runner, GLSL validation, language-aware prompt templates (11 subsystems) | ✅ **COMPLETE** |
| **3 — Overlay** | AIPromptPanel + AISuggestionPanel in editor tooling overlay | 📋 Planned |
| **3 — Code Intelligence** | clangd LSP, symbol search, AST rename, static analysis | 📋 Planned |
| **4 — Placement + PCG** | Free-move object placement; PCGLearner records transforms for learned defaults | 📋 Planned |
| **5 — GUI + Shaders** | Atlas UI widget tree generation from prompt; GLSL live compile + hot-swap | 📋 Planned |
| **6 — Asset Generation** | Blender prop/ship gen from prompts; offline image (SD), audio (AudioCraft), voice (Bark TTS) | 📋 Planned |
| **6 — Plugin System** | `plugin.json` manifests, dynamic Python loading, local plugin marketplace | 📋 Planned |
| **7 — Full Automation** | Auto test-gen, crash analysis, git integration, release pipeline, cross-platform builds | 📋 Planned |
| **7 — Multi-Agent** | Parallel agents for code, assets, tests, docs; task graph; job scheduler | 📋 Planned |

### Dev AI Source Files

**`ai_dev/core/`:** `agent_loop.py`, `context_manager.py`, `llm_interface.py`, `prompt_templates.py`

**`ai_dev/tools/`:** `build_runner.py`, `file_ops.py`, `feedback_parser.py`, `hot_reload.py`, `git_ops.py`, `ecs_scaffold.py`, `ipc_bridge.py`, `blender_bridge.py`, `pcg_learner.py`, `plugin_loader.py`, `audio_pipeline.py`, `media_pipeline.py`, `video_pipeline.py`, `stable_diffusion_interface.py`, `qa_pipeline.py`, `installer_builder.py`, `versioning.py`, `zip_tools.py`

**Tests:** 153 Python tests in `ai_dev/tests/` — `test_agent_loop.py`, `test_build_runner.py`, `test_ecs_scaffold.py`, `test_git_ops.py`, `test_hot_reload.py`, `test_ipc_bridge.py`, `test_prompt_templates.py`

**Quick Start:**
```bash
ollama pull deepseek-coder
pip install requests
cd ai_dev && python core/agent_loop.py
```

---

## 7. Blender PCG Pipeline

Located in `tools/BlenderSpaceshipGenerator/`. A full Blender addon for procedural spaceship and asset generation, wired into the NovaForge content pipeline.

### Status: ✅ Complete (Phase 7 of 7)

| Component | Status |
|-----------|--------|
| **30 addon modules** | ✅ All implemented |
| **8 PCG generators** | ✅ Galaxy, system, planet, terrain, station, ship, character, batch |
| **35 / 35 validation tests** | ✅ |
| **17 / 17 pipeline tests** | ✅ |
| **Blender 5.0 extension format** | ✅ `blender_manifest.toml` with permissions |

### Key Modules

| Module | Purpose |
|--------|---------|
| `ship_generator.py` | Spine-based hull generation (octagonal cross-section, taper) |
| `ship_parts.py` | Hull, cockpit, wings geometry |
| `station_generator.py` | Procedural station meshes |
| `interior_generator.py` | Room layouts with furniture |
| `furniture_system.py` | 7 furniture types, 12 room types, weighted placement |
| `lighting_system.py` | Interior + navigation + engine glow lights |
| `greeble_system.py` | Surface detail (panels, vents, pipes, antennas) |
| `terrain_generator.py` | Biome-based heightmaps (6 biomes) |
| `character_generator.py` | Humanoid mesh with race proportions |
| `animation_system.py` | Ship animation rigs |
| `lod_generator.py` | LOD mesh generation |
| `texture_generator.py` | Procedural textures |
| `preset_library.py` | JSON preset save/load/delete (20 parameters) |
| `template_manager.py` | Category templates with versioned metadata |
| `override_manager.py` | Manual override protection (`af_manual_override`) |
| `version_registry.py` | Semantic version tracking for 27+ generator modules |
| `atlas_exporter.py` | Export to Atlas Engine formats |

---

## 8. Design Phases A–G Status

Extracted from `docs/design/DESIGN_IDEAS_EXTRACTION.md`. Items not yet implemented are potential future systems.

### Phase A — Cinematic Warp System *(Client Rendering)*

| Feature | Status |
|---------|--------|
| Warp tunnel shader stack (5 layers) | 📋 Planned |
| Adaptive warp audio system (4 layers) | 📋 Planned |
| Dynamic warp intensity (ship mass–driven) | 📋 Planned |
| Warp anomalies (4 tiers, visual/audio only) | 📋 Planned |
| Accessibility settings (motion, bass, blur) | 📋 Planned |
| HUD "Travel Mode" (edge softening, desaturation) | 📋 Planned |

### Phase B — Fleet Personality & Social Systems *(Server ECS)*

| Feature | Status |
|---------|--------|
| Captain personality axes (4 floats) | 📋 Planned |
| Activity-aware fleet chatter | 📋 Planned |
| Interruptible chatter (priority system) | ✅ `ChatterInterruptSystem` |
| Fleet memory & morale resolution | 📋 Planned |
| Captain social graph (friendships/grudges) | ✅ `CaptainSocialGraphSystem` |
| Captain mentorship (veteran–junior bonds) | ✅ `CaptainMentorshipSystem` |
| Captain ambitions (goals, departure risk) | ✅ `CaptainAmbitionSystem` |
| Fleet emergent culture (traditions, taboos, mottos) | ✅ `FleetCultureSystem` |
| Emotional arcs across campaigns | ✅ `EmotionalArcSystem` |
| Captain departures & transfers | ✅ `FleetDepartureSystem` |
| Chatter reacting to player silence | ✅ `PlayerPresenceSystem` |
| Four faction personality profiles | ✅ `FactionBehaviorModifierSystem` |
| Positional audio in warp tunnel | 📋 Planned |

**Still needed in Phase B:** `SimCaptainPsychologyComponent`, `AtlasCaptainChatterSystem`, `AtlasFleetMoraleResolutionSystem`, `AtlasPositionalAudioSystem`

### Phase C — Fleet-as-Civilization Model *(Server ECS)*

| Feature | Status |
|---------|--------|
| Fleet cargo pool aggregator | 📋 Planned — `FleetCargoAggregatorSystem` |
| Wing system (3×5, role specialization) | 📋 Planned — `AtlasFleetDoctrineSystem` |
| Full fleet doctrine (5×5, ideology, fracture) | 📋 Planned — `AtlasFleetFractureSystem` |
| Station deployment ships | 📋 Planned — `StationDeploymentSystem` |

### Phase D — Tactical Overlay *(Client Rendering)*

| Feature | Status |
|---------|--------|
| Distance rings (toggle, projection math) | 📋 Planned — `AtlasSpatialProjectionSystem` |
| Entity projection (flat + vertical ticks) | 📋 Planned — `SimTacticalProjectionBuffer` |
| Tool range ring (active tool, color-coded) | 📋 Planned — `UI_TacticalOverlayRing` |
| Fleet anchor rings | 📋 Planned — `UI_FleetAnchorRing` |

### Phase E — Living Galaxy Simulation *(Server ECS)*

| Feature | Status |
|---------|--------|
| Star system state model (per-system floats) | 📋 Planned — `SimStarSystemState` |
| Background simulation loop | 📋 Planned — `AtlasBackgroundSimulationSystem` |
| NPC intent-driven behavior | 📋 Planned — `AtlasNPCIntentSystem` |
| Threshold-based system events | ✅ `SystemEventSystem` |
| Ambient life (shuttles, drones, beacons) | ✅ `AmbientTrafficSystem` |
| Non-combat ambient events (beacons, storms, lockdowns) | ✅ `AmbientEventSystem` |
| Debug heatmaps (threat, economy, security) | 📋 Planned — `AtlasSystemHeatmapRenderer` |

**Still needed in Phase E:** `SimStarSystemState`, `AtlasBackgroundSimulationSystem`, `AtlasNPCIntentSystem`, `AtlasSystemHeatmapRenderer`

### Phase F — Pirate Titan Meta-Threat *(Late-Game)*

| Feature | Status |
|---------|--------|
| Distributed assembly model (6 nodes) | 📋 Planned — `AtlasTitanAssemblyPressureSystem` |
| Pirate coalition AI doctrine | ✅ `FactionDoctrineSystem` |
| Galactic response curves | 📋 Planned — `AtlasGalacticResponseCurveSystem` |
| Warp evidence tied to assembly | 📋 Planned — `AtlasOuterRimLogisticsDistortionSystem` |
| Fleet chatter matrices (Titan awareness) | 📋 Planned — `AtlasRumorPropagationSystem` |

### Phase G — Additional Features *(Staged)*

| Feature | Status |
|---------|--------|
| Imperfect information / diegetic knowledge | ✅ `SimSensorConfidenceSystem` |
| Captain backgrounds (data-driven origins) | 📋 Planned — `SimCaptainBackgroundComponent` |
| Fleet norms (emergent habits) | ✅ `FleetNormSystem` |
| Persistent space scars & unofficial landmarks | ✅ `SimSpaceScarSystem` |
| Operational wear & field fixes | 📋 Planned — `SimOperationalWearSystem` |
| Behavioral reputation | 📋 Planned — `SimBehavioralReputationSystem` |
| Sector tension / slow-burn mysteries | ✅ `SimSectorTensionSystem` |
| Soft failure states (no game over) | ✅ `SimFleetFractureRecoverySystem` |
| Captain stress accumulation | ✅ `CaptainStressSystem` |
| Post-event analysis (fleet debrief) | 📋 Planned — `FleetDebriefSystem` |
| Data as gameplay (combat logs, graphs) | 📋 Planned — `UI_CombatLogExporter` |

---

## 9. EVE Feature Gap Summary

From `docs/design/EVE_FEATURE_GAP.md`. All originally identified gaps have been addressed.

### Feature Parity: ~90% of EVE's PVE mechanics

| Category | Status |
|----------|--------|
| **Sovereignty & Security Status** | ✅ Runtime AEGIS NPC spawning, gate gun AI, criminal flagging, standing effects |
| **Advanced NPC AI & Aggression** | ✅ Threat-based switching, fleet compositions, officer spawns, Sleeper AI, Drifter equivalents |
| **Bounty & Reward System** | ✅ Automatic bounty payout, LP system, LP stores, insurance, salvaging mechanics |
| **Wormhole & Null-Sec Mechanics** | ✅ Wormhole generation, mass/time limits, Sleeper caches, bubbles/cynos, dynamic anomaly spawning |
| **Market & Economy** | ✅ Order matching engine, broker fees, order modification/cancellation, regional price differences, market API |
| **Clone & Medical System** | ✅ Clone grades, relay mechanics, implants (5 attribute slots), death mechanics, relay clone installation UI |
| **Planetary Operations (PI)** | ✅ Planet scanning, resource extraction, production chains, customs offices |
| **Incursions & Live Events** | ✅ Dynamic spawning, multiple difficulty tiers, fleet coordination rewards, influence mechanics |
| **Advanced Fitting & Modules** | ✅ Tech II variants, faction drops, Deadspace loot, meta level system, implants |
| **Industry & Manufacturing** | ✅ ME/TE research, invention, reactions, capital components |
| **Character Customization** | ✅ Ship skins, character portraits, corporation logos, structure skins |
| **Social Features (Enhanced)** | ✅ Alliance system, war declarations, citadels, asset safety |
| **Advanced Movement / EWAR** | ✅ MWD signature bloom, AB mechanics, inertia modifiers, warp bubbles, webs, ECM, dampeners, tracking disruption, target painters, warp scramblers |
| **Abyssal Deadspace** | ✅ Filament entry, time-limited pockets, escalating difficulty, mutaplasmids |
| **Logistics & Support** | ✅ Remote shield/armor/hull repair, fleet command bursts |

### Intentionally Out of Scope (not EVE clones)

- ❌ PvP mechanics (ganking, faction warfare, sov warfare)
- ❌ Extreme grind (months-long skill training, capital requirements)
- ❌ Market PvP (manipulation, scam contracts)

---

## 10. Documentation & Data

### Documentation (211 markdown files across 21 subdirectories)

| Directory | Contents |
|-----------|----------|
| `docs/` (root) | ROADMAP, BUILDING, CODING_GUIDELINES, CONTRIBUTING, ARCHITECTURE_COMPARISON, EDITOR_TOOLS, EVE_MANUAL_REFERENCE, MODDING_GUIDE, REFACTORING_PLAN, STANDINGS_SYSTEM, TUTORIAL, and more |
| `docs/design/` | 23 design documents: MASTER_DESIGN_BIBLE, EVE_FEATURE_GAP, PROCEDURAL_SYSTEMS, NOVADEV_AI, DESIGN_IDEAS_EXTRACTION, chat-system-spec, editor_tool_layer, pcg_framework, server_gui_design, test_solar_system, and more |
| `docs/archive/` | NEXT_TASKS history, CHAT_LOG, ideas and improvements (6664 lines) |
| `docs/architecture/` | System architecture documents |
| `docs/guides/` | How-to guides |
| `docs/features/` | Feature specifications |
| `docs/game_mechanics/` | Gameplay mechanics reference |

### Game Data (`data/`)

JSON content files covering:
- Ships, modules, skills, market items
- Missions, contracts, bounties
- NPC factions, schedules, AI behaviors
- Security/AEGIS, insurance
- Character creation (clones, implants, portraits)
- Planetary industry, manufacturing blueprints
- Wormhole classes, anomaly types
- Corporation structures, alliances

---

## 11. Gaps Summary — What's Missing

This section lists all known gaps, ordered by priority.

### 🔴 High Priority Gaps

#### 1. Editor Tool Layer — 27 header-only tools need `.cpp` implementations
The single biggest gap. The P0 tools are needed to make the Dev Solar System editable in-game:

| Tool | Why It Matters |
|------|---------------|
| `MultiSelectionManager` | Cannot select/group multiple objects |
| `SnapAlignTool` | No grid snapping when placing assets |
| `PrefabLibrary` | Cannot drag-drop prefabs into scene |
| `CameraViewTool` | No free-fly/orbit/ortho switching |
| `LiveEditMode` | Cannot edit while simulation is running |

#### 2. Dev Solar System — Scene needs expansion
`loadTestSystem()` in `solar_system_scene.cpp` only loads Sun + 3 planets + 2 asteroid belts. Still needed:
- [ ] Stations, ships, rigs, props, characters loaded into scene
- [ ] ToolingLayer tools wired to scene entities
- [ ] DeltaEdits storage + PCG baseline propagation
- [ ] Category filtering in selection panel
- [ ] Environment preset panel (Zero-G, Low-G, Earth-Like, Windy)

#### 3. Client–Server Integration
Not all 452 ECS systems are wired to the C++ client rendering and UI. The server has full simulation depth but the client doesn't yet expose all of it to the player. Needed:
- [ ] All server system outputs piped to HUD/panels
- [ ] Full multiplayer lobby / server browser (currently a TODO)
- [ ] Interaction dispatch to nearest entity (currently a TODO)
- [ ] Interpolation delay values wired to jitter buffer (currently a TODO)

### 🟡 Medium Priority Gaps

#### 4. NovaForge Dev AI — Phases 3–7 not started
| Phase | Gap |
|-------|-----|
| 3 | No in-editor AI panel overlay (AIPromptPanel, AISuggestionPanel) |
| 3 | No clangd LSP integration (no symbol search, no AST rename) |
| 4 | PCGLearner does not yet record real placement data |
| 5 | No GLSL live compile + hot-swap from LLM prompts |
| 6 | No offline asset generation pipeline (Stable Diffusion, AudioCraft, Bark TTS) |
| 7 | No auto test-gen, crash analysis, or release pipeline |

#### 5. Design Phase B — 4 systems still planned

| System | Purpose |
|--------|---------|
| `SimCaptainPsychologyComponent` | Captain personality axes (4 floats driving all behavior) |
| `AtlasCaptainChatterSystem` | Activity-aware fleet chatter routing system |
| `AtlasFleetMoraleResolutionSystem` | Fleet-wide morale resolution from individual captain states |
| `AtlasPositionalAudioSystem` | Positional audio specifically for the warp tunnel environment |

#### 6. Design Phase C — 4 systems still planned

| System | Purpose |
|--------|---------|
| `FleetCargoAggregatorSystem` | Aggregate cargo across entire fleet for logistics |
| `AtlasFleetDoctrineSystem` | 3×5 wing system with role specialization |
| `AtlasFleetFractureSystem` | 5×5 doctrine with ideology and fracture mechanics |
| `StationDeploymentSystem` | Stations that deploy their own escort/repair ships |

#### 7. Design Phase D — Tactical Overlay (all 4 planned)

| System | Purpose |
|--------|---------|
| `AtlasSpatialProjectionSystem` | Distance rings toggled by player |
| `SimTacticalProjectionBuffer` | Entity projection to flat tactical plane |
| `UI_TacticalOverlayRing` | Color-coded tool range ring on overlay |
| `UI_FleetAnchorRing` | Fleet anchor visual rings |

#### 8. Design Phase E — 4 living galaxy systems planned

| System | Purpose |
|--------|---------|
| `SimStarSystemState` | Per-system float state model for background sim |
| `AtlasBackgroundSimulationSystem` | Background simulation loop driving system state |
| `AtlasNPCIntentSystem` | Unified NPC intent dispatcher |
| `AtlasSystemHeatmapRenderer` | Debug heatmaps (threat, economy, security) |

### 🟢 Low Priority Gaps

#### 9. Design Phase F — Pirate Titan Meta-Threat (4 of 5 planned)

| System | Purpose |
|--------|---------|
| `AtlasTitanAssemblyPressureSystem` | 6-node distributed titan assembly model |
| `AtlasGalacticResponseCurveSystem` | Faction response curves as titan nears completion |
| `AtlasOuterRimLogisticsDistortionSystem` | Warp evidence tied to titan logistics |
| `AtlasRumorPropagationSystem` | Fleet chatter matrix for Titan awareness |

#### 10. Design Phase G — 4 additional systems planned

| System | Purpose |
|--------|---------|
| `SimCaptainBackgroundComponent` | Data-driven captain origin stories |
| `SimOperationalWearSystem` | Ship wear and field-repair system |
| `SimBehavioralReputationSystem` | NPC reputation from observed player behavior |
| `FleetDebriefSystem` | Post-engagement fleet debrief / analysis |
| `UI_CombatLogExporter` | Export combat logs as playable data graphs |

#### 11. Phase A — Cinematic Warp (all 6 planned, client-side)

All Phase A features are rendering/shader work in the client, none yet implemented.

#### 12. Steam / Distribution

- [ ] Packaging and installers (Windows/Linux)
- [ ] Steam integration (Steamworks SDK)
- [ ] Cross-platform builds (Android, WASM via Emscripten)

---

## 12. Recommended Next Steps

In priority order based on the current roadmap:

### Immediate (Unblocks Vertical Slice)

1. **Implement P0 Editor Tools** (5 tools): `MultiSelectionManager`, `SnapAlignTool`, `PrefabLibrary`, `CameraViewTool`, `LiveEditMode`
2. **Expand Dev Solar System**: Add stations, ships, props, characters to `loadTestSystem()`; wire ToolingLayer to scene entities; add DeltaEdits storage
3. **Wire remaining client–server systems**: Ensure all server system outputs are exposed in the HUD/panels/network protocol

### Short-term (First Playable)

4. **Implement P1 Editor Tools** (6 tools): `AnimationEditorTool`, `IKRigTool`, `SimulationStepController`, `EnvironmentControlTool`, `MaterialShaderTool`, `LightingControlTool`
5. **Fix 4 client TODOs**: multiplayer lobby, entity interaction dispatch, error dialog, interpolation delay wiring
6. **Phase B server systems**: `SimCaptainPsychologyComponent`, `AtlasCaptainChatterSystem`, `AtlasFleetMoraleResolutionSystem`
7. **Vertical Slice integration test**: Full end-to-end in Asakai — fly, dock, mine, trade, fight, warp

### Medium-term (Alpha Quality)

8. **Phase C fleet systems**: `FleetCargoAggregatorSystem`, wing doctrine, fleet fracture
9. **Phase D tactical overlay**: Distance rings, entity projection, tool range rings
10. **Dev AI Phase 3**: In-editor AI panel overlay (AIPromptPanel, AISuggestionPanel)
11. **Performance pass**: 20 Hz server with 500+ entities; 60 FPS client with 200 ships

### Long-term (Beta / Release)

12. **Phase E living galaxy**: `SimStarSystemState`, background simulation, NPC intent dispatcher
13. **Phase F Pirate Titan**: Assembly pressure, galactic response, rumor propagation
14. **Dev AI Phases 4–7**: PCG learning, shader generation, asset pipeline, multi-agent
15. **Phase A cinematic warp**: Shader stack, adaptive audio, warp anomalies
16. **Steam integration and packaging**

---

## Quick Reference — Build Commands

```bash
# Server tests (fast iteration)
cd cpp_server && mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DUSE_STEAM_SDK=OFF
cmake --build . -j$(nproc)
./bin/test_systems                          # all server tests

# Single system test
cmake --build . --target test_<name>_system -j$(nproc)
./bin/test_<name>_system

# Atlas Engine tests
cd build_tests
cmake .. -DBUILD_CLIENT=OFF -DBUILD_SERVER=OFF -DBUILD_ATLAS_ENGINE=ON \
         -DBUILD_ATLAS_TESTS=ON
cmake --build . --target AtlasTests -j$(nproc)
./bin/AtlasTests                            # 915 assertions

# Blender PCG pipeline tests
cd tools/BlenderSpaceshipGenerator
python test_validation.py                   # 35/35 passing
python pcg_pipeline/test_pipeline.py        # 17/17 passing

# NovaForge Dev AI
cd ai_dev && python core/agent_loop.py

# Dev AI Python tests
python -m unittest discover -s ai_dev/tests -v    # 153 tests

# Full project build
./scripts/build_all.sh --test
```

---

*Last updated: March 24, 2026*

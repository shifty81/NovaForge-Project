# Nova Forge — Project Roadmap

> **Consolidated roadmap** — For deep design detail, see the [Master Design Bible](design/MASTER_DESIGN_BIBLE.md).
> Archived planning docs are in [`docs/archive/`](archive/).
> **Offline AI dev assistant design:** see [`docs/design/NOVADEV_AI.md`](design/NOVADEV_AI.md).

---

## Project Vision

Nova Forge is a **PvE-focused space simulator** for solo play or small groups (2–20 players), built on the custom **Atlas Engine** (C++/OpenGL). The game delivers EVE-like depth — ships, skills, fitting, combat, missions, exploration — powered by an AI-driven economy where every NPC is a real economic actor. The simulation is server-authoritative, tick-based (10–20 Hz), and deterministic for networking and replays. All UI is rendered through the custom Atlas UI retained-mode system with an EVE-style sci-fi dark theme. Content is fully moddable via JSON.

---

## Architecture

| Layer | Responsibility |
|-------|---------------|
| **Server (Authoritative)** | Tick-based simulation, ECS, AI, economy, combat, persistence |
| **Client (Display)** | Rendering, interpolation, UI, input, audio — no game logic authority |
| **Networking** | Server snapshots → Client cache → Interpolation → Render |

---

## Design Enforcement Rules (Non-Negotiable)

Every new system or feature **must** pass all three checks:

1. **Atlas-fit** — Does it belong naturally inside Atlas Engine?
2. **Simulation-first** — Does it work without UI, audio, or player input?
3. **Lore-consistent** — Would captains in-universe believe this exists?

If it fails any one → it doesn't ship.

---

## Systems Overview

| System | Key Subsystems | Status |
|--------|---------------|--------|
| **NovaForge Dev AI** | AgentLoop, LocalLLM, FileOps, BuildRunner, HotReload, PCGLearner, Overlay Panels | 📋 Planned — P0 |
| **Atlas Engine Core** | ECS, Deterministic Scheduler, EventBus, Plugin System | ✅ Complete |
| **Networking** | TCP Client/Server, Interpolation, Delta Compression, Snapshot Replication, Jitter Buffer | ✅ Complete |
| **Rendering** | OpenGL, Deferred Pipeline, Shadow Mapping, Post-Processing | ✅ Complete |
| **Atlas UI** | Widgets, Panels, Docking (DockNode tree), Photon Dark Theme | ✅ Complete |
| **Atlas Editor** | 21 Panels, PCG Tools, Dock Layout, Debug Console | ✅ Complete |
| **Editor Tool Layer** | 32 ITool implementations, EditorCommandBus, UndoableCommandBus, DeltaEditStore | ✅ Complete |
| **Ship HUD** | Control Ring (shield/armor/hull arcs), Capacitor Bar, Module Rack, Target Brackets | ✅ Complete |
| **PCG Ship Generation** | Spine-based Hulls, Turrets, Faction Shape Language, Procedural Materials | ✅ Complete |
| **PCG Content** | Stations, Interiors, Characters, Galaxy Generation | ✅ Complete |
| **Combat** | Lock/Fire/Damage Layers, Wrecks, Salvage, Security Response | ✅ Complete |
| **Fleet System** | Wings (5×5), Doctrine, Morale, Chatter, Fracture Mechanics | ✅ Complete |
| **AI NPCs** | Miners, Haulers, Pirates, Traders, Security, Intent-driven Behavior Trees, NPC Schedules | ✅ Complete |
| **Economy** | Markets, Supply/Demand, Trade Routes, Dynamic Tax, Broker Fees | ✅ Complete |
| **Mining & Industry** | Ore, Ice, Refining, Manufacturing, Planetary Industry | ✅ Complete |
| **Missions** | Procedural Templates, Objectives, Rewards, Branching Chains, Consequences | ✅ Complete |
| **Exploration** | Anomalies, Wormholes, Scan Probes, Jump Gates | ✅ Complete |
| **Audio** | OpenAL, Positional 3D, Fleet Audio, Warp Adaptive Audio | ✅ Complete |
| **Physics** | Rigid Body Dynamics, AABB Collision | ✅ Complete |
| **Animation** | Graph Pipeline, Clip/Blend/Modifier/StateMachine Nodes | ✅ Complete |
| **Warp System** | Tunnel Shader Stack, Adaptive Audio, Mass-based Intensity, Anomalies | ✅ Complete |
| **Titan & Meta-Threat** | Assembly Pressure, Logistics Distortion, Rumor Propagation, Galactic Response | ✅ Complete |
| **FPS & Interiors** | EVA Airlock, Salvage Paths, Lavatory, Rig Locker, Clone Bay | ✅ Complete |
| **Planetary** | Space→Planet Transition, Rover Bay, Grid Construction, Hangars | ✅ Complete |
| **Community & Modding** | Content Repository, Validation, Mission Editor, Mod Manager, Doc Generator | ✅ Complete |
| **Vertical Slice Integration** | Full star system: fly, fight, mine, trade, dock — end-to-end | 🔧 In Progress |
| **Client Polish & Optimization** | Profiling, Spatial Partitioning, 500+ Entity Performance | 🔧 In Progress |
| **Steam / Distribution** | Packaging, Installer, Steam Integration | 📋 Planned |

---

## Development Phases

| Phase | Name | Version | Status |
|-------|------|---------|--------|
| **0** | Foundation — Engine, rendering, ECS, basic flight, HUD, audio | v0.1 | ✅ Complete |
| **1** | Core Fleet — Player + 4 captains, fitting, mining, warp, stations | v0.1–v0.2 | ✅ Complete |
| **2** | Living Universe — Background sim, NPC life, economy engine, combat aftermath | v0.2 | ✅ Complete |
| **3** | Wing System & Midgame — 3×5 wings, doctrine, tactical overlay, imperfect info | v0.3 | ✅ Complete |
| **4** | Full Fleet Doctrine & Endgame — 5×5 fleet, ideology, fracture, persistence | v0.4+ | ✅ Complete |
| **5** | Titan Systems & Meta-Threat — Galactic emergent threats, warp anomalies | v0.5+ | ✅ Complete |
| — | Vertical Slice — One full star system, end-to-end playable | — | 🔧 In Progress |
| — | Client Integration & Polish — Smooth networking, full HUD, performance | — | 🔧 In Progress |
| **Dev AI** | NovaForge Dev AI — Offline AI assistant, hot-reload, iterative dev loop | — | 📋 **Active Priority** |

---

## Current Priorities

1. **⭐ NovaForge Dev AI — Phase 0 (Offline Dev Loop)** — Get a local LLM running with the agent loop so you can prompt the AI to generate/fix code, compile, and iterate — all offline. See [`docs/design/NOVADEV_AI.md`](design/NOVADEV_AI.md) for the full plan.
2. **Complete Tooling Layer** — Implement the 27 header-only editor tools so the Dev Solar System is fully editable in-game
3. **Dev Solar System** — First playable star system with all PCG asset types, editable via ToolingLayer, DeltaEdits propagation live
4. **Client–Server Integration** — Wire up all server systems to C++ client rendering and UI
5. **Vertical Slice** — One full star system: fly, dock, mine, trade, fight, warp — end-to-end playable
6. **Performance Profiling** — Maintain 20 Hz server tick with 500+ entities; 60 FPS client with 200 visible ships
7. **Atlas UI Polish** — All game panels using Atlas UI docking system with EVE-style theme
8. **Network Smoothness** — Interpolation, jitter buffer, and lag compensation feel solid at 100ms latency
9. **AI Economic Actors Live** — Miners, haulers, pirates creating visible supply/demand cycles
10. **Content Balance Pass** — Missions, rewards, difficulty scaling tuned for solo and co-op play

---

## ⭐ NovaForge Dev AI — Offline AI Development Assistant (TOP PRIORITY)

> **Full design spec:** [`docs/design/NOVADEV_AI.md`](design/NOVADEV_AI.md)
> **Source ideas:** [`docs/archive/CHAT_LOG.md`](archive/CHAT_LOG.md)

NovaForge Dev AI is the **ultimate offline AI development studio** built
directly into this project — a local LLM agent that prompts, codes, compiles,
hot-reloads, generates assets, and iterates, all from inside the engine.

The Atlas Engine is already in this repo (`engine/` + `editor/`).
The editor already has `AIAggregator` + `TemplateAIBackend` in `editor/ai/`.
`LocalLLMBackend.cpp` (now added) wires it to Ollama for real LLM responses.

### What "Done" Looks Like — Phase 0 (Running Now)

1. `ollama pull deepseek-coder` + `python ai_dev/core/agent_loop.py`
2. Prompt → LLM reads project context → suggests file changes
3. User approves → FileOps applies → CMake builds → errors fed back to LLM
4. Full loop: prompt → code → build → fix → iterate — all offline

### Full Implementation Roadmap

| Phase | Goal | Status |
|-------|------|--------|
| **0 — Seed Baseline** | Agent loop, local LLM, file ops, CMake runner, error parser | ✅ **COMPLETE** |
| **1 — Hot Reload** | File watcher, Lua/Python/GLSL/JSON selective reload | 🔧 In Progress (stubs done) |
| **2 — Multi-Language** | C#/Java runners, language-aware prompt templates | 📋 Planned |
| **3 — Overlay** | AIPromptPanel + AISuggestionPanel in tooling overlay | 📋 Planned |
| **3 — Code Intelligence** | clangd LSP, symbol search, AST rename, static analysis | 📋 Planned |
| **4 — Placement + PCG** | Free-move object placement, PCG learns approved layouts | 📋 Planned |
| **5 — GUI + Shaders** | Atlas UI panel generation, GLSL live compile + hot-swap | 📋 Planned |
| **6 — Asset Generation** | Blender prop/ship gen from prompts, offline image/audio gen | 📋 Planned |
| **6 — Plugin System** | stable_diffusion, audiocraft, bark_tts, vision plugins | 📋 Planned |
| **7 — Full Automation** | Auto test-gen, crash analysis, git integration, release pipeline | 📋 Planned |
| **7 — Multi-Agent** | Parallel agents for code, assets, tests, docs | 📋 Planned |

### Complete Feature List (from Chat)

**Core Agent:** prompt loop, local LLM (Ollama/LM Studio), project file index,
session memory, file snapshot/rollback, CMake build runner, multi-language
runners (Python/Lua/Blender/GLSL), GCC/Clang/MSVC error parser, linker error
extraction, error→fix→rebuild loop.

**Hot Reload:** file change watcher, Lua dofile, Python importlib.reload, GLSL
recompile+link, JSON re-parse, Atlas UI panel re-register, C++ DLL hot-swap.

**In-Editor Overlay:** AIPromptPanel, AISuggestionPanel (diff + confirm),
AIBuildLogPanel, AIContextPanel, all wired through EditorCommandBus/
UndoableCommandBus for full undo/redo safety.

**Code Intelligence:** clangd LSP integration, symbol/reference search,
AST-safe rename, clang-tidy static analysis, tree-sitter index, SQLite
per-project symbol DB, semantic search via LLM embeddings.

**Placement + PCG Learning:** AI places objects at learned defaults, user
free-moves to final position, PCGLearner records transform + room context,
future rooms use learned data for smarter default placements.

**GUI + Shader Design:** Atlas UI widget tree generation from prompt, GLSL
fragment shader generation + hot-compile, color/font/hover effect tweaks live,
style saved back to panel definition files.

**Asset Generation:** Blender prop/ship generation via Python API, LLM-driven
geometry scripts, `.glb`/`.fbx` export, offline image generation (Stable
Diffusion), tileable textures, UI icons (SDXL/ComfyUI), audio SFX
(AudioCraft), NPC voiceovers (Bark TTS), reference image analysis (LLaVA).

**Plugin System:** `plugin.json` manifests, dynamic Python module loading,
tool registry, local plugin marketplace in `ai_dev/plugins/`.

**Full Automation:** auto ECS test generation, profiler integration
(perf/Valgrind), crash/minidump analysis (dbghelp/Breakpad), telemetry log
analysis, Git integration (stage/commit/push/branch), release pipeline
(cmake → strip → zip → NSIS installer), cross-platform builds
(Linux/Android/WASM via Emscripten), multi-process build farm, Windows PE
toolchain (icons/manifests/resources), documentation auto-generation.

**Multi-Agent:** parallel agent instances for code + assets + testing + docs,
task graph/build graph system, job scheduler for large engine builds.

### Quick-Start

```bash
# 1. Install Ollama
curl -fsSL https://ollama.com/install.sh | sh
ollama pull deepseek-coder   # or: codellama:13b, qwen2.5-coder

# 2. Python deps
pip install requests

# 3. Run
cd NovaForge/ai_dev
python core/agent_loop.py
```

```
NovaForge Dev AI> Show me all ECS systems missing a .cpp implementation.
NovaForge Dev AI> Add a stub FleetDebriefSystem following the existing pattern.
NovaForge Dev AI> build
NovaForge Dev AI> The Upgrade button in EquipmentPanel does nothing. Fix it.
NovaForge Dev AI> Generate a futuristic console prop, 1x1x2m, sci-fi style.
NovaForge Dev AI> snapshot before_refactor
NovaForge Dev AI> rollback
```

---

## New Vertical Slice Goal: Tooling Layer + Dev Solar System

> **Primary focus**: Complete the in-game editor tooling and the first developer
> solar system so that assets can be manually tweaked in-game to smooth the
> look of things. This is the current highest-priority milestone.

### What "Done" Looks Like

1. All 32 editor tools have working `.cpp` implementations (27 remain header-only).
2. The Dev Solar System loads with at least one of every PCG asset type
   (planet, station, ship, rig, prop, character, asteroid belt).
3. Every asset is selectable, transformable, and editable via the ToolingLayer.
4. DeltaEdits persist changes and propagate to PCG baseline.
5. Physics simulation can be paused, stepped, and scrubbed.
6. The system is accessible only in editor mode (`NOVAFORGE_EDITOR_TOOLS=ON`).

### Tooling Layer — Implementation Status

**Implemented (5 of 32 tools):**

| Tool | Header | Implementation |
|------|--------|---------------|
| EditorCommandBus | `editor_command_bus.h` | `editor_command_bus.cpp` ✅ |
| EditorEventBus | `editor_event_bus.h` | `editor_event_bus.cpp` ✅ |
| EditorToolLayer | `editor_tool_layer.h` | `editor_tool_layer.cpp` ✅ |
| SceneBookmarkManager | `scene_bookmark_manager.h` | `scene_bookmark_manager.cpp` ✅ |
| UndoableCommandBus | `undoable_command_bus.h` | `undoable_command_bus.cpp` ✅ |

**Header-Only — Need Implementation (27 tools):**

| Priority | Tool | Header | Purpose |
|----------|------|--------|---------|
| P0 | MultiSelectionManager | `multi_selection_manager.h` | Select/group multiple assets |
| P0 | SnapAlignTool | `snap_align_tool.h` | Grid/surface/asset alignment |
| P0 | PrefabLibrary | `prefab_library.h` | Drag-and-drop reusable modules |
| P0 | CameraViewTool | `camera_view_tool.h` | Free-fly, orbit, ortho views |
| P0 | LiveEditMode | `live_edit_mode.h` | Edit while playing |
| P1 | AnimationEditorTool | `animation_editor_tool.h` | Multi-layer animation editing |
| P1 | IKRigTool | `ik_rig_tool.h` | Inverse kinematics for characters/turrets |
| P1 | SimulationStepController | `simulation_step_controller.h` | Pause/step/scrub simulation |
| P1 | EnvironmentControlTool | `environment_control_tool.h` | Gravity, wind, atmosphere |
| P1 | MaterialShaderTool | `material_shader_tool.h` | Live material/shader editing |
| P1 | LightingControlTool | `lighting_control_tool.h` | Light property editing + presets |
| P2 | FunctionAssignmentTool | `function_assignment_tool.h` | Assign triggers to entities |
| P2 | EventTimelineTool | `event_timeline_tool.h` | Scripted sequence editor |
| P2 | NPCSpawnerTool | `npc_spawner_tool.h` | AI/PCG asset spawning |
| P2 | MapEditorTool | `map_editor_tool.h` | Star system map editing |
| P2 | ShipModuleEditorTool | `ship_module_editor_tool.h` | Ship module slot editing |
| P2 | ResourceBalancerTool | `resource_balancer_tool.h` | Resource distribution |
| P3 | PCGSnapshotManager | `pcg_snapshot_manager.h` | Snapshot/rollback PCG state |
| P3 | DeltaEditsMergeTool | `delta_edits_merge_tool.h` | Merge/resolve DeltaEdits |
| P3 | EditPropagationTool | `edit_propagation_tool.h` | Propagate changes to similar assets |
| P3 | VisualDiffTool | `visual_diff_tool.h` | Compare current vs baseline |
| P3 | LayerTagSystem | `layer_tag_system.h` | Asset categorization + visibility |
| P3 | AssetStatsPanel | `asset_stats_panel.h` | Hierarchy/physics/memory stats |
| P3 | HotkeyActionManager | `hotkey_action_manager.h` | Custom keybind assignment |
| P3 | ScriptConsole | `script_console.h` | Real-time debug console |
| P3 | BatchOperationsTool | `batch_operations_tool.h` | Mass transformations |
| P0 | ITool (base) | `itool.h` | Base interface (header-only by design) |

### Dev Solar System — Implementation Status

**Existing infrastructure:**
- `cpp_client/include/core/solar_system_scene.h` — Scene class with Celestial struct (8 types: Sun, Planet, Moon, Station, Stargate, AsteroidBelt, Wormhole, Anomaly)
- `cpp_client/src/core/solar_system_scene.cpp` — Test system "Asakai" with Sun + 3 planets + 2 asteroid belts
- `docs/design/test_solar_system.md` — Full design spec with JSON blueprint
- Server-side: `star_system_generator`, `star_system_manager_system`, `star_system_populator_system`

**Still needed for the Dev Solar System:**
1. Expand `loadTestSystem()` to include all PCG asset types (stations, ships, rigs, props, characters)
2. Wire ToolingLayer tools to solar system scene entities
3. Implement DeltaEdits storage and PCG baseline propagation
4. Add category filtering in selection panel (Planets, Stations, Ships, etc.)
5. Add environment preset panel (Zero-G, Low-G, Earth-Like, Windy)
6. Test with real PCG-generated assets from Blender pipeline

---

## Explicitly Out of Scope

- ❌ PvP combat
- ❌ Player empires / sovereignty
- ❌ Full MMO economy simulation
- ❌ Scripted storylines / quest chains
- ❌ Twitch shooter mechanics
- ❌ Clickable tactical overlays
- ❌ Fake distances or rubber-band difficulty
- ❌ Asset-copied EVE clone
- ❌ Client-authoritative logic
- ❌ Theme-park content

---

## Development Milestones

| Milestone | Target | Criteria |
|-----------|--------|----------|
| **Engine Baseline** | ✅ Done | ECS, rendering, audio, physics, networking, UI framework |
| **All Server Systems** | ✅ Done | 21,626+ test assertions passing across 434+ ECS systems |
| **Atlas Engine Modules** | ✅ Done | 915 engine test assertions (input, camera, audio, animation, plugin) |
| **Editor Tool Layer** | ✅ Headers | 32 tool headers, 5 have .cpp; 27 need implementation |
| **Blender PCG Pipeline** | ✅ Done | 30 addon modules, 35/35 validation tests, 17/17 pipeline tests |
| **⭐ Dev AI Phase 0** | ✅ **COMPLETE** | Agent loop + LLM + build runner working offline (`python ai_dev/core/agent_loop.py`) |
| **Dev AI Phase 1** | ✅ **COMPLETE** | Auto-iterate, test runner, git ops, ECS scaffold, CLI mode, session resume |
| **Dev AI Phase 2** | ✅ **COMPLETE** | IPC bridge, hot-reload wired, C#/.NET runner, GLSL validation, prompt templates |
| **Dev AI Phase 3** | 📋 Planned | AI prompt panel + suggestion panel in tooling overlay |
| **Dev AI Phase 4–7** | 📋 Planned | Placement PCG, GUI/shader gen, asset pipeline, multi-agent |
| **Complete Tooling Layer** | 🔧 In Progress | All 32 tools with working .cpp implementations |
| **Dev Solar System** | 📋 Next | All PCG asset types editable in-game via ToolingLayer |
| **Vertical Slice** | 📋 Planned | One star system fully playable end-to-end |
| **Alpha** | TBD | 2–4 players, smooth networking, core gameplay loop |
| **Beta** | TBD | 8+ players, full UI, content variety, performance targets met |
| **Release** | TBD | Packaged builds, mod support, documentation |

---

## Design Ideas — Implementation Roadmap

> Extracted from [`docs/design/DESIGN_IDEAS_EXTRACTION.md`](design/DESIGN_IDEAS_EXTRACTION.md).
> Source material: `docs/archive/ideas and improvements.txt`.

### Phase A — Cinematic Warp System (Client Rendering)

| Feature | Subsystem | Priority | Status |
|---------|-----------|----------|--------|
| Warp tunnel shader stack (5 layers) | AtlasRender | High | 📋 Planned |
| Adaptive warp audio system (4 layers) | AtlasAudio | High | 📋 Planned |
| Dynamic warp intensity (ship mass–driven) | SimWarpProfile | High | 📋 Planned |
| Warp anomalies (4 tiers, visual/audio only) | AtlasWarpAnomalyEvidenceSystem | Medium | 📋 Planned |
| Accessibility settings (motion, bass, blur) | AtlasWarpAccessibility | Medium | 📋 Planned |
| HUD "Travel Mode" (edge softening, desaturation) | UI_HUDTravelMode | Medium | 📋 Planned |

### Phase B — Fleet Personality & Social Systems (Server ECS)

| Feature | Subsystem | Priority | Status |
|---------|-----------|----------|--------|
| Captain personality axes (4 floats) | SimCaptainPsychologyComponent | High | 📋 Planned |
| Activity-aware fleet chatter | AtlasCaptainChatterSystem | High | 📋 Planned |
| Interruptible chatter (priority system) | ChatterInterruptSystem | High | ✅ Done |
| Fleet memory & morale | AtlasFleetMoraleResolutionSystem | High | 📋 Planned |
| Captain social graph (friendships/grudges) | CaptainSocialGraphSystem | Medium | ✅ Done |
| Captain mentorship (veteran–junior bonds) | CaptainMentorshipSystem | Medium | ✅ Done |
| Captain ambitions (long-term goals, departure risk) | CaptainAmbitionSystem | Medium | ✅ Done |
| Fleet emergent culture (traditions, taboos, mottos) | FleetCultureSystem | Low | ✅ Done |
| Emotional arcs across campaigns | EmotionalArcSystem | Medium | ✅ Done |
| Captain departures & transfers | FleetDepartureSystem | Medium | ✅ Done |
| Chatter reacting to player silence | PlayerPresenceSystem | Low | ✅ Done |
| Four faction personality profiles | FactionBehaviorModifierSystem | Low | ✅ Done |
| Positional audio in warp tunnel | AtlasPositionalAudioSystem | Low | 📋 Planned |

### Phase C — Fleet-as-Civilization Model (Server ECS)

| Feature | Subsystem | Priority | Status |
|---------|-----------|----------|--------|
| Fleet cargo pool aggregator | FleetCargoAggregatorSystem | High | 📋 Planned |
| Wing system (3×5, role specialization) | AtlasFleetDoctrineSystem | Medium | 📋 Planned |
| Full fleet doctrine (5×5, ideology, fracture) | AtlasFleetFractureSystem | Low | 📋 Planned |
| Station deployment ships | StationDeploymentSystem | Low | 📋 Planned |

### Phase D — Tactical Overlay (Client Rendering)

| Feature | Subsystem | Priority | Status |
|---------|-----------|----------|--------|
| Distance rings (toggle, projection math) | AtlasSpatialProjectionSystem | High | 📋 Planned |
| Entity projection (flat + vertical ticks) | SimTacticalProjectionBuffer | High | 📋 Planned |
| Tool range ring (active tool, color-coded) | UI_TacticalOverlayRing | Medium | 📋 Planned |
| Fleet anchor rings | UI_FleetAnchorRing | Low | 📋 Planned |

### Phase E — Living Galaxy Simulation (Server ECS)

| Feature | Subsystem | Priority | Status |
|---------|-----------|----------|--------|
| Star system state model (per-system floats) | SimStarSystemState | High | 📋 Planned |
| Background simulation loop | AtlasBackgroundSimulationSystem | High | 📋 Planned |
| NPC intent-driven behavior | AtlasNPCIntentSystem | High | 📋 Planned |
| Threshold-based system events | SystemEventSystem | Medium | ✅ Done |
| Ambient life (shuttles, drones, beacons) | AmbientTrafficSystem | Medium | ✅ Done |
| Non-combat ambient events (beacons, storms, lockdowns) | AmbientEventSystem | Medium | ✅ Done |
| Debug heatmaps (threat, economy, security) | AtlasSystemHeatmapRenderer | Low | 📋 Planned |

### Phase F — Pirate Titan Meta-Threat (Server ECS, Late-Game)

| Feature | Subsystem | Priority | Status |
|---------|-----------|----------|--------|
| Distributed assembly model (6 nodes) | AtlasTitanAssemblyPressureSystem | Low | 📋 Planned |
| Pirate coalition AI doctrine | AtlasFactionDoctrineSystem | Low | ✅ Done |
| Galactic response curves | AtlasGalacticResponseCurveSystem | Low | 📋 Planned |
| Warp evidence tied to assembly | AtlasOuterRimLogisticsDistortionSystem | Low | 📋 Planned |
| Fleet chatter matrices (Titan awareness) | AtlasRumorPropagationSystem | Low | 📋 Planned |

### Phase G — Additional Features (Staged, Low-Risk)

| Feature | Subsystem | Priority | Status |
|---------|-----------|----------|--------|
| Imperfect information / diegetic knowledge | SimSensorConfidenceSystem | Medium | ✅ Done |
| Captain backgrounds (data-driven origins) | SimCaptainBackgroundComponent | Medium | 📋 Planned |
| Fleet norms (emergent habits) | FleetNormSystem | Low | ✅ Done |
| Persistent space scars & unofficial landmarks | SimSpaceScarSystem | Low | ✅ Done |
| Operational wear & field fixes | SimOperationalWearSystem | Low | 📋 Planned |
| Behavioral reputation | SimBehavioralReputationSystem | Low | 📋 Planned |
| Sector tension / slow-burn mysteries | SimSectorTensionSystem | Low | ✅ Done |
| Soft failure states (no game over) | SimFleetFractureRecoverySystem | Low | ✅ Done |
| Captain stress accumulation (combat, near-deaths, deployment) | CaptainStressSystem | Low | ✅ Done |
| Post-event analysis (fleet debrief) | FleetDebriefSystem | Low | 📋 Planned |
| Data as gameplay (combat logs, graphs) | UI_CombatLogExporter | Low | 📋 Planned |

---

## Naming Conventions

| Scope | Prefix | Example |
|-------|--------|---------|
| Engine-wide | `Atlas` | `AtlasDeterministicScheduler` |
| Simulation | `Sim` | `SimFleetMoraleComponent` |
| Rendering | `AtlasRender` | `AtlasRenderTacticalOverlay` |
| UI | `UI_` | `UI_TacticalOverlayRing` |
| Game-specific | `EVO_` | `EVO_PirateCoalitionDoctrine` |

**Forbidden:** Generic names (`Manager`, `Controller`, `Handler`), engine-agnostic names (`InventorySystem`).
Components = pure data, no logic, serializable. Systems = stateless logic, deterministic, scheduled by Atlas ECS.

---

*For detailed system designs, see the [Master Design Bible](design/MASTER_DESIGN_BIBLE.md).*
*Archived planning documents: [`docs/archive/`](archive/)*

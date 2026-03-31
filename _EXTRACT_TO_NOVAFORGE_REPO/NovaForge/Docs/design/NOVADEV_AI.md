# NovaForge Dev AI — Complete Design Specification

> **Priority: P0 — Active Development**
> Fully offline, in-project AI development assistant and asset generation
> platform, embedded into the NovaForge Atlas Engine and tooling overlay.
> Source ideas: [`docs/archive/CHAT_LOG.md`](../archive/CHAT_LOG.md)

---

## Vision

NovaForge Dev AI is the **ultimate offline AI development studio** built
directly into this project.  It combines:

- A **local LLM agent loop** (prompt → plan → code → compile → iterate)
- **In-engine overlay integration** (Alt+F12 Dev Mode — edit while playing)
- **Hot reload** for Lua, Python, GLSL, JSON, and Atlas UI panels
- **PCG placement learning** (AI learns your style and suggests layouts)
- **3D/2D asset generation** via Blender Python API and offline SD models
- **Plugin/extension system** for adding new capabilities without touching core
- **Code intelligence** (LSP, AST, symbol search) for large C++ repos
- **Automated build/test/release pipeline** from prompt to packaged installer
- **Multi-agent mode** — parallel agents for code, assets, testing, and docs
- **Git integration** — commit, branch, push, and rollback AI-applied changes
- **Crash/telemetry analysis** — feed minidumps and logs to AI for fixes
- **Cross-platform build targets** — Windows, Linux, Android, Web (Emscripten)

Everything runs **100% offline** using a local LLM (Ollama / LM Studio).

---

## Atlas Engine Integration

The Atlas Engine is **already in this repository** at `engine/` and `editor/`.
The editor already has an `ai/` subsystem with `AIAggregator` + `TemplateAIBackend`.
NovaForge Dev AI extends this with `LocalLLMBackend` (now in `editor/ai/`) which
provides real LLM responses and is automatically selected by the aggregator when
Ollama is running.

```
editor/ai/
├── AIAggregator.h/.cpp       ← existing aggregator (unchanged)
├── TemplateAIBackend.h/.cpp  ← existing keyword templates (unchanged)
└── LocalLLMBackend.h/.cpp    ← NEW: Ollama / LM Studio bridge
```

---

## Repository Layout

```
NovaForge/
├── engine/         ← Atlas Engine (ECS, rendering, audio, physics, net)
├── editor/         ← Atlas Editor  (panels, AI backend, PCG tools)
│   └── ai/
│       ├── AIAggregator.cpp
│       ├── TemplateAIBackend.cpp
│       └── LocalLLMBackend.cpp   ← NEW
├── cpp_server/     ← Game server (ECS systems, simulation)
├── cpp_client/     ← Game client (OpenGL, Atlas UI, HUD)
├── tools/          ← Blender addon, PCG pipeline tools
└── ai_dev/         ← Offline AI dev assistant (Python)
    ├── core/
    │   ├── agent_loop.py        ← Main prompt loop + CLI + auto-iterate
    │   ├── llm_interface.py     ← Ollama / LM Studio API
    │   └── context_manager.py   ← Project file indexer
    ├── tools/
    │   ├── file_ops.py          ← Read / write / snapshot
    │   ├── build_runner.py      ← CMake / Python / Lua / Blender runners
    │   ├── feedback_parser.py   ← Compiler error → structured AI input
    │   ├── git_ops.py           ← Git status / diff / commit / branch
    │   ├── ecs_scaffold.py      ← ECS system boilerplate generator
    │   ├── hot_reload.py        ← Selective reload without restart
    │   ├── blender_bridge.py    ← 3D asset generation
    │   ├── pcg_learner.py       ← Placement learning database
    │   └── plugin_loader.py     ← Plugin / extension system
    ├── tests/                   ← Unit tests for AI dev tools
    ├── plugins/                 ← Optional extensions (drop in to enable)
    ├── workspace/               ← Session logs, snapshots, memory
    └── models/                  ← LLM model download instructions
```

---

## Full Feature Set (All Chat Ideas)

### A. Core Agent Loop

| Feature | Status | File |
|---------|--------|------|
| Prompt → plan → execute loop | ✅ Done | `ai_dev/core/agent_loop.py` |
| Local LLM interface (Ollama) | ✅ Done | `ai_dev/core/llm_interface.py` |
| LM Studio (OpenAI-compat) support | ✅ Done | `ai_dev/core/llm_interface.py` |
| Project file indexer | ✅ Done | `ai_dev/core/context_manager.py` |
| Keyword-based context scoring | ✅ Done | `ai_dev/core/context_manager.py` |
| Per-project AI memory (JSON) | ✅ Done | `ai_dev/core/context_manager.py` |
| Session history (multi-turn chat) | ✅ Done | `ai_dev/core/agent_loop.py` |
| Session resume (reload on startup) | ✅ Done | `ai_dev/core/agent_loop.py` |
| File snapshot before any edit | ✅ Done | `ai_dev/tools/file_ops.py` |
| Rollback to any snapshot | ✅ Done | `ai_dev/tools/file_ops.py` |
| CMake build runner + log capture | ✅ Done | `ai_dev/tools/build_runner.py` |
| Python / Lua / Blender runners | ✅ Done | `ai_dev/tools/build_runner.py` |
| GCC/Clang/MSVC error parser | ✅ Done | `ai_dev/tools/feedback_parser.py` |
| Linker error symbol extraction | ✅ Done | `ai_dev/tools/feedback_parser.py` |
| Error → LLM fix → rebuild loop | ✅ Done | `ai_dev/core/agent_loop.py` |
| Auto-iterate (hands-free build loop) | ✅ Done | `ai_dev/core/agent_loop.py` |
| Test runner (per-system or all) | ✅ Done | `ai_dev/core/agent_loop.py` |
| CLI / batch mode (non-interactive) | ✅ Done | `ai_dev/core/agent_loop.py` |
| Auto-approve mode | ✅ Done | `ai_dev/core/agent_loop.py` |
| ECS system scaffolding | ✅ Done | `ai_dev/tools/ecs_scaffold.py` |
| Git integration (status/diff/commit) | ✅ Done | `ai_dev/tools/git_ops.py` |
| Status dashboard | ✅ Done | `ai_dev/core/agent_loop.py` |
| Missing systems listing | ✅ Done | `ai_dev/core/agent_loop.py` |
| C++ LocalLLMBackend (editor) | ✅ Done | `editor/ai/LocalLLMBackend.cpp` |

### B. Hot Reload (Phase 1 + Phase 2)

| Feature | Status | File |
|---------|--------|------|
| File watcher (polling thread) | ✅ Done | `ai_dev/tools/hot_reload.py` |
| Lua hot reload (via IPC) | ✅ Done | `ai_dev/tools/hot_reload.py` |
| Python module reload | ✅ Done | `ai_dev/tools/hot_reload.py` |
| GLSL shader recompile (via IPC) | ✅ Done | `ai_dev/tools/hot_reload.py` |
| JSON data re-parse (via IPC) | ✅ Done | `ai_dev/tools/hot_reload.py` |
| IPC bridge to engine process | ✅ Done | `ai_dev/tools/ipc_bridge.py` |
| Atlas UI panel hot-reload (via IPC) | ✅ Done | `ai_dev/tools/hot_reload.py` |
| Batch reload + statistics | ✅ Done | `ai_dev/tools/hot_reload.py` |
| C++ DLL reload (Windows) | 📋 Phase 3 | build + LoadLibrary swap |

### C. Multi-Language Build Support (Phase 2)

| Language | Builder | Status |
|----------|---------|--------|
| C++ (server/client) | CMake + Ninja | ✅ Done |
| Python (tools/AI) | python interpreter | ✅ Done |
| Lua (scripts) | lua / luajit | ✅ Done |
| GLSL (shaders) | glslangValidator | ✅ Done |
| Blender (PCG pipeline) | blender --python | ✅ Done |
| C# (.NET) | dotnet build | ✅ Done |
| Java | javac / gradle | 📋 Phase 2 |

### D. In-Engine Overlay Integration (Phase 3)

These panels extend the **existing tooling overlay** (Alt+F12 toggle) by
registering as editor tools through `EditorCommandBus`.

| Panel | Purpose | Status |
|-------|---------|--------|
| `AIPromptPanel` | Natural language input, send to agent | 📋 Phase 3 |
| `AISuggestionPanel` | Diff view of proposed changes + confirm/reject | 📋 Phase 3 |
| `AIBuildLogPanel` | Streaming build output, errors highlighted | 📋 Phase 3 |
| `AIContextPanel` | Active files, session history, memory notes | 📋 Phase 3 |
| `AIHotReloadPanel` | Watch list, reload triggers, file change feed | 📋 Phase 3 |

Integration points with existing infrastructure:

| System | Hook |
|--------|------|
| `EditorCommandBus` | All AI changes emitted as editor commands (undo/redo safe) |
| `UndoableCommandBus` | AI changes fully undoable |
| `DeltaEditStore` | Structural AI edits stored as delta edits |
| `LiveEditMode` | AI hot-reloads execute inside live edit mode |
| `ScriptConsole` | Build log and AI output streamed here |
| `AIAggregator` | `LocalLLMBackend` registered as highest-confidence backend |

### E. Interactive Object Placement + PCG Learning (Phase 4)

| Feature | Status | File |
|---------|--------|------|
| Placement record storage (JSON) | ✅ Done | `ai_dev/tools/pcg_learner.py` |
| Centroid-based placement suggestions | ✅ Done | `ai_dev/tools/pcg_learner.py` |
| Style tag recording & querying | ✅ Done | `ai_dev/tools/pcg_learner.py` |
| Free-Move placement mode (overlay) | 📋 Phase 4 | editor tool extension |
| AI-generated placeholder at default | 📋 Phase 4 | overlay + BlenderBridge |
| Transform save after user approval | 📋 Phase 4 | PCGSnapshotManager hook |

**Workflow:**
```
Prompt: "Add a crafting console to this room"
→ AI places placeholder at learned default position
→ User enters Free-Move, repositions it
→ User approves → pcg_learner records transform + context
→ Next room: AI suggests the learned position automatically
```

### F. GUI & Shader Live Design (Phase 5)

| Feature | Status |
|---------|--------|
| Atlas UI panel generation from prompt | 📋 Phase 5 |
| Widget tree hot-reload | 📋 Phase 5 |
| GLSL shader live compilation | 📋 Phase 5 (stub in hot_reload.py) |
| Shader_live_editor.py | 📋 Phase 5 |
| Color / font / hover effect tweaking | 📋 Phase 5 |
| Style save back to panel definition | 📋 Phase 5 |

### G. 3D Asset Generation Pipeline (Phase 6)

| Feature | Status | File |
|---------|--------|------|
| Blender prop generation (placeholder cube) | ✅ Stub | `ai_dev/tools/blender_bridge.py` |
| Blender ship generation (addon bridge) | ✅ Stub | `ai_dev/tools/blender_bridge.py` |
| `.glb` / `.fbx` export | ✅ Stub | `ai_dev/tools/blender_bridge.py` |
| LLM-driven geometry generation (Phase 6) | 📋 Phase 6 | replace stub in bridge |
| Reference image analysis (LLaVA offline) | 📋 Phase 6 | plugin: `vision_model` |
| Interactive approval loop in overlay | 📋 Phase 6 | overlay panel |
| Tileable texture generation (offline SD) | 📋 Phase 6 | plugin: `stable_diffusion` |
| UI icon generation (SDXL/ComfyUI) | 📋 Phase 6 | plugin: `stable_diffusion` |
| Audio SFX generation (AudioCraft/Bark) | 📋 Phase 6 | plugin: `audiocraft` |
| NPC voiceover generation (Bark TTS) | 📋 Phase 6 | plugin: `bark_tts` |

### H. Plugin / Extension System (Phase 1+)

| Feature | Status | File |
|---------|--------|------|
| Plugin manifest (plugin.json) | ✅ Done | `ai_dev/tools/plugin_loader.py` |
| Dynamic module loading | ✅ Done | `ai_dev/tools/plugin_loader.py` |
| Tool registry (name → run fn) | ✅ Done | `ai_dev/tools/plugin_loader.py` |
| Plugin directory scan | ✅ Done | `ai_dev/tools/plugin_loader.py` |
| Local plugin marketplace README | ✅ Done | `ai_dev/plugins/README.md` |
| Built-in plugins (SD, AudioCraft, Bark) | 📋 Phase 6 | plugins/ |

### I. Code Intelligence (Phase 3)

| Feature | Tool | Status |
|---------|------|--------|
| Symbol search | ripgrep + clangd | 📋 Phase 3 |
| Find definition / references | clangd LSP | 📋 Phase 3 |
| AST-safe rename | clang AST | 📋 Phase 3 |
| Static analysis / linting | clang-tidy | 📋 Phase 3 |
| Per-project symbol index (SQLite) | tree-sitter | 📋 Phase 3 |
| Semantic code search | LLM embedding | 📋 Phase 3 |

### J. Full Dev Studio Automation (Phase 7)

| Feature | Status |
|---------|--------|
| Auto test generation for new ECS systems | 📋 Phase 7 |
| Performance profiling integration (perf/Valgrind) | 📋 Phase 7 |
| Crash minidump analysis (dbghelp/Breakpad) | 📋 Phase 7 |
| Telemetry log analyzer (feed logs to AI) | 📋 Phase 7 |
| Git integration: stage, commit, push, branch | 📋 Phase 7 |
| Release pipeline: build → strip → package | 📋 Phase 7 |
| ZIP/7z packaging | 📋 Phase 7 |
| Windows installer (NSIS/WiX) | 📋 Phase 7 |
| Cross-platform targets (Linux, Android, WASM) | 📋 Phase 7 |
| Multi-agent mode (parallel agents) | 📋 Phase 7 |
| Build farm / multi-process compilation | 📋 Phase 7 |
| Windows PE toolchain (icons, manifests, resources) | 📋 Phase 7 |
| Documentation auto-generation | 📋 Phase 7 |
| In-editor AI-assisted tutorials | 📋 Phase 7 |

---

## Phase Implementation Roadmap

### Phase 0 — Seed Baseline ✅ COMPLETE

**Goal:** Get a local AI running that can read prompts, generate code, compile,
and iterate — all from a terminal.

- [x] `ai_dev/core/agent_loop.py` — prompt → plan → execute loop
- [x] `ai_dev/core/llm_interface.py` — Ollama + LM Studio API wrapper
- [x] `ai_dev/core/context_manager.py` — project file indexer + memory
- [x] `ai_dev/tools/file_ops.py` — read / write / snapshot / rollback
- [x] `ai_dev/tools/build_runner.py` — CMake + script runners + log capture
- [x] `ai_dev/tools/feedback_parser.py` — GCC/Clang/MSVC/linker error parser
- [x] `editor/ai/LocalLLMBackend.h/.cpp` — C++ Ollama bridge for in-editor AI
- [x] `editor/CMakeLists.txt` — LocalLLMBackend registered
- [x] `ai_dev/README.md` and `ai_dev/models/README.md`

### Phase 1 — Iteration Tools ✅ COMPLETE

**Goal:** Enable hands-free iterate-and-build cycles so the AI can fix its own
build errors, scaffold new systems, and track changes via git — all offline.

- [x] **Auto-iterate mode** — `auto <prompt>` loops: prompt → code → build →
      parse errors → re-prompt LLM → fix → rebuild (up to N rounds)
- [x] **Test runner** — `test` and `test <name>` commands build and run tests
- [x] **CLI / batch mode** — `--prompt`, `--auto`, `--auto-approve` CLI flags
      for non-interactive scripted usage
- [x] **ECS scaffold** — `scaffold <name> [component_file]` generates header,
      .cpp, test, and component snippet in one command
- [x] **Git integration** — `git status/diff/log/commit/branch/stash` commands
      for tracking AI-applied changes without leaving the agent
- [x] **Status dashboard** — `status` shows branch, LLM health, build state,
      and missing system count at a glance
- [x] **Missing systems** — `missing` lists systems without .cpp or test files
- [x] **Session resume** — loads previous session history on startup
- [x] **Session clear** — `clear` command to start fresh
- [x] **Context invalidation** — applied file changes are immediately re-indexed
- [x] `ai_dev/tools/git_ops.py` — git status, diff, commit, branch, stash
- [x] `ai_dev/tools/ecs_scaffold.py` — ECS system boilerplate generator
- [x] `ai_dev/tests/` — 57 unit tests for new modules

### Phase 2 — Hot Reload + Multi-Language Build Runners

- [x] `ai_dev/tools/hot_reload.py` — file watcher + dispatch (stubs ready)
- [x] Wire IPC signals from HotReloadManager to engine process
- [x] `ai_dev/tools/ipc_bridge.py` — TCP socket IPC client for engine reload commands
- [ ] GLSL recompile hook in Atlas renderer (engine-side; IPC client ready)
- [ ] JSON re-parse hook in game server (engine-side; IPC client ready)
- [x] Atlas UI panel hot-reload path (IPC protocol defined)
- [x] C++ CMake runner (Phase 0)
- [x] Python / Lua / Blender runners (Phase 0)
- [x] GLSL validation runner (`validate_glsl_dir` in BuildRunner)
- [x] C# / dotnet runner (`run_dotnet_build`, `run_dotnet_test`)
- [x] Language-aware prompt templating per subsystem (`ai_dev/core/prompt_templates.py`)
- [x] Hot-reload batch mode + statistics tracking
- [x] Agent loop: `watch` / `reload` commands

### Phase 3 — Overlay Integration

- [ ] `editor/tools/AIPromptPanel.h/.cpp`
- [ ] `editor/tools/AISuggestionPanel.h/.cpp` (diff view + confirm)
- [ ] `editor/tools/AIBuildLogPanel.h/.cpp` (streaming output)
- [ ] Register panels with `EditorCommandBus`
- [ ] Wire AI-applied changes through `UndoableCommandBus`
- [ ] Code intelligence: clangd LSP bridge, symbol search plugin

### Phase 4 — Interactive Placement + PCG Learning

- [x] `ai_dev/tools/pcg_learner.py` (ready)
- [ ] Free-Move mode in tooling overlay
- [ ] Connect PCGLearner to `PCGSnapshotManager`
- [ ] AI default-placement based on learned data

### Phase 5 — GUI + Shader Design

- [ ] `ai_dev/tools/shader_live_editor.py`
- [ ] Atlas UI widget tree generator (prompt → panel JSON → hot-reload)
- [ ] GLSL live-compile path in shader_live_editor.py

### Phase 6 — Asset Generation

- [x] `ai_dev/tools/blender_bridge.py` (placeholder stubs ready)
- [ ] LLM-driven Blender geometry scripts (replace cube placeholders)
- [ ] `plugins/stable_diffusion/` — offline texture generation
- [ ] `plugins/audiocraft/` — SFX and music generation
- [ ] `plugins/bark_tts/` — NPC voiceover generation
- [ ] Reference image analysis via local LLaVA model
- [ ] Interactive approval loop in overlay

### Phase 7 — Full Dev Studio

- [ ] Automated ECS test generation
- [ ] Git tool plugin
- [ ] Release pipeline: cmake → strip → zip → NSIS installer
- [ ] Crash/minidump analysis plugin
- [ ] Multi-agent mode (concurrent agent instances)
- [ ] Cross-platform build targets (Linux/Android/WASM)

---

## Quick-Start: Phase 0 + Phase 1 (Running Right Now)

### 1. Install Ollama and pull a model

```bash
curl -fsSL https://ollama.com/install.sh | sh
ollama pull deepseek-coder
```

### 2. Install Python deps

```bash
pip install requests
```

### 3. Run the agent loop

```bash
cd NovaForge/ai_dev
python core/agent_loop.py
```

### 4. Example session (interactive)

```
NovaForge Dev AI> status
NovaForge Dev AI> missing
NovaForge Dev AI> scaffold fleet_debrief fleet_components.h
NovaForge Dev AI> build
NovaForge Dev AI> auto Fix all build errors in test_fleet_debrief_system
NovaForge Dev AI> git commit "AI: add fleet_debrief_system"
NovaForge Dev AI> test test_fleet_debrief_system
```

### 5. Example session (CLI / non-interactive)

```bash
# Single prompt
python core/agent_loop.py --prompt "Add a stub FleetDebriefSystem"

# Full auto-iterate with build loop
python core/agent_loop.py --auto "Add FleetDebriefSystem with tests" --auto-approve -n 10
```

---

## Design Principles

1. **Human in the loop** — AI always presents proposed changes before any file
   is modified.  Every change requires explicit approval (`yes` / `no`).
2. **Snapshot before edit** — Every touched file is snapshotted automatically.
   Rollback is always available.
3. **Offline first** — All LLM inference runs locally via Ollama or LM Studio.
   No cloud API keys or internet connection required.
4. **Engine-native** — The primary UI is the existing NovaForge tooling overlay.
   All AI commands route through `EditorCommandBus` for undo/redo safety.
5. **Incremental** — Phase 0 works with just Python + Ollama.  Phases 1–7 layer
   on top without breaking the baseline.
6. **Atlas-fit** — All in-engine hooks follow Atlas Editor, Atlas UI, and ECS
   patterns established throughout the engine/ directory.
7. **Plugin-extensible** — New capabilities (image gen, audio gen, crash
   analysis) are added as plugins in `ai_dev/plugins/` without touching core.

---

## Recommended Local Models

| Model | Size | Best for |
|-------|------|----------|
| `deepseek-coder:6.7b` | ~4 GB | Fast C++ iteration (recommended start) |
| `deepseek-coder:33b` | ~20 GB | High-quality C++ + architecture |
| `codellama:13b` | ~8 GB | General code, Python, C++ |
| `qwen2.5-coder:7b` | ~5 GB | Multi-language |
| `mistral:7b` | ~5 GB | General reasoning |
| `mixtral:8x7b` | ~26 GB | Best quality complex tasks |

See [`ai_dev/models/README.md`](../../ai_dev/models/README.md) for full details.

---

*For roadmap position, see [`docs/ROADMAP.md`](../ROADMAP.md).*
*Source material: [`docs/archive/CHAT_LOG.md`](../archive/CHAT_LOG.md).*

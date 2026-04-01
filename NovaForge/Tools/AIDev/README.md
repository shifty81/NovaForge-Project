# NovaForge Dev AI — Quick-Start Guide

Fully offline, in-project AI development assistant for NovaForge.
All inference runs locally via [Ollama](https://ollama.com) or LM Studio — no
internet required after the initial model download.

---

## Prerequisites

**Python 3.8+** required. Python 3.10+ recommended.

### 1. Install Ollama (recommended)
```bash
curl -fsSL https://ollama.com/install.sh | sh
```

### 2. Pull a coding model
```bash
# Best results for C++:
ollama pull deepseek-coder

# Alternatives:
ollama pull codellama:13b
ollama pull qwen2.5-coder:7b
ollama pull mistral:7b
```

### 3. Python dependencies
```bash
pip install requests
```

---

## Running the Agent Loop

### Interactive mode (default)

```bash
cd NovaForge/ai_dev
python core/agent_loop.py
```

The agent loop starts an interactive prompt session.  Type `help` to see all
commands, or `exit` to quit.

### CLI mode (non-interactive)

```bash
# Single prompt — get a response and exit
python core/agent_loop.py --prompt "Show me all ECS systems missing a .cpp"

# Auto-iterate — prompt → code → build → fix → rebuild (up to N rounds)
python core/agent_loop.py --auto "Add a stub FleetDebriefSystem with tests" --auto-approve

# Control iteration depth
python core/agent_loop.py --auto "Fix the linker errors" -n 10 --auto-approve
```

---

## Commands

| Command | Description |
|---------|-------------|
| `<prompt>` | Send a natural language prompt to the AI |
| `build` | Run cmake build and optionally send errors to AI |
| `test [name]` | Build and run tests (all, or a specific test binary) |
| `auto <prompt>` | Auto-iterate: prompt → code → build → fix → repeat |
| `scaffold <name> [comp]` | Generate ECS system stubs (header, .cpp, test) |
| `status` | Show project dashboard (git, LLM, IPC, build, missing) |
| `missing` | List ECS systems missing .cpp or test files |
| `git <subcmd>` | Git operations (status/diff/log/commit/branch/stash) |
| `watch [path] [type]` | Watch a file for hot-reload (or list watched files) |
| `reload <path>` | Manually trigger hot-reload for a file |
| `snapshot [label]` | Snapshot the workspace |
| `rollback` | Roll back files to a previous snapshot |
| `history` | Show session's prompt/response history |
| `clear` | Clear session history |
| `help` | Show help message |
| `exit` | Exit the agent loop |

---

## Example Sessions

### Basic prompt flow
```
NovaForge Dev AI> Show me all ECS systems missing a .cpp implementation.
NovaForge Dev AI> Add a stub FleetDebriefSystem following the existing pattern.
NovaForge Dev AI> build
NovaForge Dev AI> The linker fails on test_fleet_debrief_system. Fix it.
```

### Auto-iterate (hands-free)
```
NovaForge Dev AI> auto Add a FleetDebriefSystem with full test coverage and register in CMakeLists
[Auto] Starting auto-iterate (max 5 rounds)...
[Auto] Round 1/5 — querying LLM...
[Auto] Applying 4 file changes...
[Auto] Building...
[Auto] Build succeeded!
[Auto] Running tests...
[Auto] All tests passed! Auto-iterate complete.
```

### ECS scaffolding
```
NovaForge Dev AI> scaffold fleet_debrief fleet_components.h
[Scaffold] Generating fleet_debrief (component file: fleet_components.h)...
  ✓ cpp_server/include/systems/fleet_debrief_system.h
  ✓ cpp_server/src/systems/fleet_debrief_system.cpp
  ✓ cpp_server/tests/test_fleet_debrief_system.cpp
```

### Git integration
```
NovaForge Dev AI> git status
NovaForge Dev AI> git diff
NovaForge Dev AI> git commit "AI: add fleet_debrief_system"
```

### Hot-reload / file watching (Phase 2)
```
NovaForge Dev AI> watch assets/scripts/equip.lua
[Watch] Now watching: assets/scripts/equip.lua [auto]

NovaForge Dev AI> watch shaders/pbr.frag glsl
[Watch] Now watching: shaders/pbr.frag [glsl]

NovaForge Dev AI> watch
[Watch] 2 file(s) watched:
  [lua   ] assets/scripts/equip.lua  (reloaded 0x)
  [glsl  ] shaders/pbr.frag          (reloaded 0x)

NovaForge Dev AI> reload assets/scripts/equip.lua
[Reload] ✓ Reloaded assets/scripts/equip.lua [lua]
```

---

## What Happens Under the Hood

```
Your prompt
  ↓
ContextManager reads relevant source files
  ↓
LLM generates suggested changes
  ↓
You approve / reject / show (or auto-approve in CLI mode)
  ↓
FileOps applies changes (after snapshotting originals)
  ↓
BuildRunner runs: cmake --build cpp_server/build -j$(nproc)
  ↓
FeedbackParser structures compiler output
  ↓
If errors → LLM refines and repeats (auto-iterate loop)
If success → done
```

---

## Implementation Phases

See [docs/design/NOVADEV_AI.md](../docs/design/NOVADEV_AI.md) for the full
7-phase design specification.

| Phase | Goal | Status |
|-------|------|--------|
| 0 | Seed Baseline — agent loop, LLM, file ops, build runner | ✅ Complete |
| 1 | Iteration Tools — auto-iterate, test runner, git ops, scaffold, CLI mode | ✅ Complete |
| 2 | Hot Reload + Multi-Lang — IPC bridge, C#/.NET, GLSL validation, prompt templates | ✅ Complete |
| 3 | Overlay — AIPromptPanel + AISuggestionPanel in tooling overlay | 📋 Planned |
| 4 | Placement + PCG — free-move placement, PCG learning | 📋 Planned |
| 5 | GUI + Shaders — Atlas UI / GLSL generation on the fly | 📋 Planned |
| 6 | Asset Generation — Blender bridge for props from prompts | 📋 Planned |
| 7 | Full Automation — test-gen, release pipeline, multi-agent | 📋 Planned |

---

## Running Tests

```bash
cd NovaForge
python -m unittest discover -s ai_dev/tests -v
```

---

## Models

See [`models/README.md`](models/README.md) for recommended model selection and
hardware requirements.

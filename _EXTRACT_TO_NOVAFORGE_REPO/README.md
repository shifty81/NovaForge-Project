# NovaForge — Extracted from ATlasToolingSuite

This folder contains **everything needed to create the standalone `NovaForge` repository**.
It was extracted from `shifty81/ATlasToolingSuite` as part of the Suite Decoupling Architecture (Phase 71+).

---

## What Is In This Folder

| Sub-folder | Contents | Destination in NovaForge repo |
|---|---|---|
| `NovaForge/` | **Full game source** — C++ client, server, gameplay, world, UI, save, CMakeLists | root of repo → `NovaForge/` → or directly as repo root |
| `Docs/NovaForge/` | NovaForge-specific design docs | `Docs/` |
| `Docs/Runtime/` | Expanded runtime/gameplay spec | `Docs/` |
| `Docs/NovaForge_First_Live_Ingestion_Plan.md` | Ingestion planning doc | `Docs/` |
| `AtlasSuite_Runtime_Bridge/NovaForge/` | C# runtime services that belonged to NovaForge (Builder, Salvage, Vehicles, Constructs). These were inside `Atlas/UI/AtlasSuite/Runtime/NovaForge/` | `Integrations/AtlasSuite/Runtime/` or wherever the NovaForge repo puts its Atlas-side bridge files |
| `AtlasSuite_Suite_Contamination/Modules.Project/` | `NovaForgeDefinitionService`, `NovaForgeLevel`, `LevelEntity`, `LevelService`, `PatchApplicator` — NovaForge-specific implementations that were incorrectly in the Suite's `AtlasSuite.Modules.Project` | `Integrations/AtlasSuite/Adapter/` |
| `AtlasSuite_Suite_Contamination/Modules.Engine/` | `BuildNovaForgeCommand.cs` — NovaForge-specific build command that was in the Suite | `Integrations/AtlasSuite/Adapter/` |
| `AtlasSuite_Suite_Contamination/Plugin.Sample/` | `SampleSalvagePlugin.cs` — NovaForge Salvage plugin that was in the Suite's sample plugin project | `Integrations/AtlasSuite/Plugins/` |
| `AtlasAI_ProjectAdapter/NovaForge/` | Python AtlasAI project adapter for NovaForge (`NovaForgeProjectAdapter`, `BridgeRequestEnvelope`, live ingestion client) | `AtlasAI/ProjectAdapters/NovaForge/` |

---

## How to Create the NovaForge Repository

### Step 1 — Create a new GitHub repo
```
https://github.com/shifty81/NovaForge   (suggested name)
```

### Step 2 — Initialise with the extracted content
```bash
# From this folder:
cp -r NovaForge/ <path-to-new-novaforge-repo>/
cp -r Docs/         <path-to-new-novaforge-repo>/Docs/
cp -r AtlasSuite_Runtime_Bridge/NovaForge/ <path-to-new-novaforge-repo>/Integrations/AtlasSuite/Runtime/
cp -r AtlasSuite_Suite_Contamination/      <path-to-new-novaforge-repo>/Integrations/AtlasSuite/Adapter/
cp -r AtlasAI_ProjectAdapter/NovaForge/    <path-to-new-novaforge-repo>/AtlasAI/ProjectAdapters/NovaForge/
```

### Step 3 — Verify the NovaForge CMake tree is intact
```bash
cd <path-to-new-novaforge-repo>
cmake -B build -DCMAKE_BUILD_TYPE=Debug .   # root is NovaForge/CMakeLists.txt
```

### Step 4 — Add the bridge manifest
The `Projects/NovaForge/novaforge.project.json` file **stays in `ATlasToolingSuite`**.
It is the single connection point between the Suite and the NovaForge game.
Do NOT copy it into this repo.

### Step 5 — Remove this `_EXTRACT_TO_NOVAFORGE_REPO/` folder from ATlasToolingSuite
Once the standalone NovaForge repo is live and verified, delete this staging folder:
```bash
git rm -r _EXTRACT_TO_NOVAFORGE_REPO/
git commit -m "chore: remove NovaForge extraction staging folder — NovaForge now lives at github.com/shifty81/NovaForge"
```

---

## What Stays in ATlasToolingSuite After Extraction

| Remains in ATlasToolingSuite | Reason |
|---|---|
| `Projects/NovaForge/novaforge.project.json` | Bridge manifest — Suite reads this to locate the game |
| `AtlasAI/ProjectAdapters/IProjectAdapter.cs` | Generic interface — not NovaForge-specific |
| `Atlas/UI/AtlasSuite/RuntimeScaffold/` | The Suite itself — fully generic now |
| `Shared/ProjectManifests/novaforge.project.json` | Legacy copy — can be removed once bridge is stable |

---

## Bridge Protocol (How Suite ↔ NovaForge Connect)

The Atlas Suite connects to NovaForge via:
1. **Project manifest** — Suite reads `Projects/NovaForge/novaforge.project.json` at startup
2. **HTTP bridge** — `AtlasAiBridgeService` sends tool calls to `http://localhost:8765`
3. **No shared assemblies** — Suite never loads NovaForge DLLs; all communication is over the bridge

This means NovaForge can be on any machine, any OS, and deployed independently.

---

## Checklist Before Deleting This Folder

- [ ] New NovaForge repo created at `github.com/shifty81/NovaForge`
- [ ] All content from this staging folder committed to the new repo
- [ ] NovaForge CMake builds successfully standalone
- [ ] AtlasAI ProjectAdapter committed to NovaForge repo
- [ ] Bridge roundtrip tested (Suite → JSON manifest → bridge endpoint → NovaForge)
- [ ] `_EXTRACT_TO_NOVAFORGE_REPO/` folder deleted from ATlasToolingSuite and committed

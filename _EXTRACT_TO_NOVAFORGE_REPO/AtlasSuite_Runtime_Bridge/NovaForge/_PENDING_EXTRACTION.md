# PENDING EXTRACTION — NovaForge Runtime (Phase 71)

This directory contains **NovaForge-specific runtime code** that is currently
inside the Atlas Suite shell.  This is a **boundary violation** — the Suite must
not contain project-specific logic.

## Target location

All content here should move to `Projects/NovaForge/` (or the NovaForge project
adapter in `AtlasAI/ProjectAdapters/NovaForge/`) as part of the ongoing
**Phase 71 — Suite Decoupling Architecture**.

## Affected directories

| Current path | Target |
|---|---|
| `Atlas/UI/AtlasSuite/Runtime/NovaForge/Builder/` | `Projects/NovaForge/Runtime/Builder/` |
| `Atlas/UI/AtlasSuite/Runtime/NovaForge/Constructs/` | `Projects/NovaForge/Runtime/Constructs/` |
| `Atlas/UI/AtlasSuite/Runtime/NovaForge/DevWorld/` | `Projects/NovaForge/Runtime/DevWorld/` |
| `Atlas/UI/AtlasSuite/Runtime/NovaForge/Salvage/` | `Projects/NovaForge/Runtime/Salvage/` |
| `Atlas/UI/AtlasSuite/Runtime/NovaForge/Vehicles/` | `Projects/NovaForge/Runtime/Vehicles/` |

## Why this matters

The Suite is a **generic editor platform**.  Projects connect to it via the
project manifest (`Projects/NovaForge/novaforge.project.json`) and the bridge
protocol.  No project-specific assemblies should live inside the Suite.

See `Docs/Architecture/repo_boundaries.md` for the boundary rules.

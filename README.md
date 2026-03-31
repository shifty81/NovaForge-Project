# NovaForge

NovaForge is the standalone game project hosted by Atlas Suite.

This repository contains the game project only. It does not own Atlas Suite shell behavior, generic tooling, generic editor hosts, repo migration engines, or workspace-level services.

## Initial Goal
Establish a clean repo that:
- follows Atlas Suite hosted-project requirements
- keeps tooling out of the game repo
- supports controlled migration from legacy NovaForge sources
- supports phased Copilot-assisted buildout

## Top-Level Layout
```text
NovaForge/
  novaforge.project.json
  README.md
  ARCHITECTURE.md
  PROJECT_RULES.md
  BUILD_RULES.md
  ROADMAP.md
  CONTENT_RULES.md
  TASKS.md
  Docs/
    Specs/
    Migration/
  Intake/
    manifests/
    reports/
    staging/
  Source/
  Content/
  Config/
  Scripts/
  Tests/
  Packaging/
  Release/
```

## First Steps
1. Review the root rule files.
2. Review `Docs/Specs/`.
3. Use `Scripts/bootstrap_repo.ps1` or `Scripts/bootstrap_repo.sh` locally.
4. Keep legacy source zips outside the repo, or place them under a gitignored migration source folder.
5. Only import NovaForge-approved files through manifests and staged migration.

## Migration Rule
Legacy source archives are migration inputs, not permanent repo contents.

# Hosted Project Contract

NovaForge exists inside Atlas Suite as a hosted game project. Atlas Suite is the host environment. NovaForge is the hosted product.

## Atlas Suite owns
- shell and workspace environment
- project loading and registration
- generic editor hosts
- generic content intake framework
- generic build orchestration
- generic logging infrastructure
- generic AI/debug broker workflows
- generic release pipeline framework

## NovaForge owns
- game runtime
- project data and content definitions
- project-specific schemas and inspectors
- project-specific validation rules
- project build configuration
- client and server startup definitions
- project packaging metadata
- project tests
- project release metadata

## Required Top-Level Layout
```text
NovaForge/
  novaforge.project.json
  README.md
  Docs/
  Source/
  Content/
  Config/
  Scripts/
  Tests/
  Packaging/
  Release/
```

# Adapter Migration Checklist

## Phase 2.5 Goals
- ingest real legacy data safely
- keep runtime architecture clean
- avoid importing old engine/render/input loops
- translate legacy formats into Master Repo-owned data

## Safe-first order
1. Data definitions
2. Gameplay helpers
3. Tooling bridges
4. AI bridges
5. Higher-risk runtime behavior

## Do not import directly
- old engine kernel
- render loop
- input stack
- monolithic actors
- old networking shortcuts
- direct file writes that bypass authority/save systems

## First target systems
### NovaForge
- items
- recipes
- missions
- factions
- loot tables

### Arbiter / ArbiterAI
- task parsing concepts
- workspace context concepts
- panel/chat/workflow concepts

## Required output for each migrated system
- source repo origin
- adapter used
- new destination
- adoption status
- refactor notes

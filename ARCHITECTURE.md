# NovaForge Architecture

## Purpose
NovaForge is the standalone game project hosted by Atlas Suite.

## Non-Goals
This repository does not contain:
- Atlas Suite shell
- Atlas Suite workspace services
- generic editor host framework
- generic content intake ownership
- generic build orchestration ownership
- generic AI broker ownership
- generic repo migration tooling

## Major Domains
- Core
- World
- Gameplay
- Rig
- Building
- Salvage
- Seasons
- Factions
- AI
- UI
- Client
- Server
- Validation
- Tests

## Current Design Locks
- Voxel layer is authoritative.
- Low-poly wrapper is deferred and later reflects voxel state.
- The player suit platform is the R.I.G.
- The R.I.G. starts as a minimal exo frame with life support.
- Helmet deployment and minimal HUD are part of early progression.
- Seasons are configurable client-side and server-side, with server authority as the normal mode.

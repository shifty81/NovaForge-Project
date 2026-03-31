# MasterRepo Gameplay Foundation Pack

This pack provides the first gameplay-facing foundation layer for Master Repo.

## Included
- player state and transform types
- inventory system
- crafting system
- interaction system
- mission system
- lightweight gameplay manager
- UI hook stubs for runtime HUD / inventory / crafting / mission log
- starter player definition

## Purpose
This is the first step from:
- runtime infrastructure
- data ingestion
- registry expansion

into:

- basic playable loop foundation

## Target loop
Spawn -> Move -> Open Inventory -> Craft -> Interact -> Accept Mission -> Progress

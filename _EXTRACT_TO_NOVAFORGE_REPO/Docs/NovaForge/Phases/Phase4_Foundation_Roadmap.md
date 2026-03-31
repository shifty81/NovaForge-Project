# Phase 4 Foundation Roadmap

## Scope
This phase turns the project from an interior/runtime shell into the first recognizable EVA game loop.

## Core systems
1. Player mode state + transition controller
2. Airlock state machine with interlocks and pressure behavior
3. Tether system with oxygen/power feed model
4. Exterior survival model
5. EVA locomotion model
6. Room breach / environment linkage
7. Exterior target interaction shell

## Immediate follow-up after this pack
- wire player mode into existing gameplay manager
- connect ship interior shell room state to airlock cycle results
- add basic debug HUD output for mode / oxygen / tether state
- hook exterior target interactions into inventory/loot later

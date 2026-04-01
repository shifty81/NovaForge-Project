# Gameplay Foundation Integration

## Intended integration targets
- `World`
- `SystemScheduler`
- `DataRegistry`
- `ToolingSubsystem` or runtime UI shell
- future input system

## Recommended integration order
1. Add `GameplayManager` to `World`
2. Call `GameplayManager::Initialize(DataRegistry&)`
3. Tick it from `SystemScheduler`
4. Seed starter inventory
5. Assign starter mission
6. Route TAB to inventory/crafting UI state
7. Route interaction key to `InteractionSystem`

## Notes
This pack intentionally stays architecture-aligned:
- data-driven
- no rendering/input hard-binding
- no old legacy loops
- gameplay state separated from UI state

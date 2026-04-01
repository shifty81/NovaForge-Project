# DataRegistry Expansion Integration

## What this adds
- category-specific record models
- folder scanning for multiple definition types
- startup summary logging
- basic lookup APIs by ID
- minimal validation hooks

## Categories added
- Items
- Recipes
- Missions
- Factions
- Loot
- Modules

## Suggested next steps after integration
1. add startup validation for missing recipe item references
2. add mission/faction summary output in world boot
3. add inventory and crafting systems to query loaded data
4. add schema validation pass
5. add save/load adapters that reference these IDs

# MasterRepo Legacy Adapter Pack

This pack provides a safe adapter layer for bringing audited legacy content into Master Repo.

## Purpose
Legacy repos are mixed-content donor archives. This pack creates a **quarantine + translation layer** so old code and data can be converted into Master Repo architecture without polluting live runtime systems.

## Included
- Core adapter interfaces
- Legacy source classification helpers
- Data adapters for items, recipes, missions, factions, modules
- Gameplay bridge stubs for inventory/crafting/interaction
- AI bridge stubs for Arbiter integration
- Tooling bridge stubs for panel/chat/workspace integration
- Sample legacy JSON and converted outputs
- Migration checklist

## Integration target
Add this folder under:

```text
/Source/LegacyAdapters/
```

## Rule
Legacy systems must flow like this:

```text
Legacy Repo -> Adapter -> Master Repo Runtime/Data
```

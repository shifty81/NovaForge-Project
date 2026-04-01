# Character System Roadmap

## Core goals
1. Modular low-poly body part architecture
2. Equipment attachment system
3. Animation state framework
4. FPS / EVA / Mech movement state compatibility
5. Mech transfer / possession shell
6. Visual upgrade compatibility with progression systems

## Design rules
- keep base body simple and readable
- body parts are modular, not one dense mesh
- equipment should attach through sockets
- animation should favor rigid readable motion
- mech possession should reuse player control context

## Best next integration after this pack
- hook character system into gameplay player controller
- connect suit/equipment to inventory and upgrades
- bind animation states to movement modes
- connect mech possession to EVA/gameplay state

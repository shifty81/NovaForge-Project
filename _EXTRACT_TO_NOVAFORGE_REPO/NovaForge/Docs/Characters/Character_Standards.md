# Character Standards

## Scale
- base humanoid height target: 1.80 meters
- recommended engine unit baseline: 1 unit = 1 centimeter
- recommended voxel cell target for human-scale interaction: 50 cm to 100 cm depending on structure layer
- first-person hand/tool presentation may use local visual offsets, but gameplay collision uses world scale

## Socket naming
- socket_hand_r
- socket_hand_l
- socket_backpack
- socket_helmet
- socket_tool_mount
- socket_hip_r
- socket_hip_l

## Socket orientation rule
- forward axis must align with held/use direction
- up axis must remain consistent project-wide
- attachment offsets must be stored in data, not hardcoded per tool if avoidable

## Character pipeline
Input -> MovementIntent -> CharacterState -> Animation/IK -> Presentation -> Render

## Mode rules
- FPS -> EVA only through valid transition state
- EVA -> FPS only in valid interior/gravity-safe state
- FPS -> Mech only through valid cockpit/entry interaction
- Mech -> FPS only through safe exit state

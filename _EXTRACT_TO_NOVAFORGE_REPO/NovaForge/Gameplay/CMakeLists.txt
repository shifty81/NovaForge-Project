# NovaForge Gameplay — game systems, factions, economy, missions, combat

set(NOVAFORGE_GAMEPLAY_SOURCES
    src/NovaForgeGameplay_stub.cpp

    # Combat
    Combat/CombatSystem.cpp

    # Economy
    Economy/TradeMarket.cpp
    Economy/ResourceRegistry.cpp
    Economy/LootResolver.cpp

    # Factions
    Factions/FactionRegistry.cpp

    # Missions
    Missions/MissionRegistry.cpp

    # Mining
    Mining/MiningSystem.cpp

    # Exploration
    Exploration/ExplorationSystem.cpp

    # PCG
    PCG/PCGWorldGen.cpp

    # Progression
    Progression/ProgressionSystem.cpp

    # Player Systems
    PlayerSystems/PlayerController.cpp

    # Builder
    Builder/BuilderSystem.cpp

    # Salvage
    Salvage/SalvageSystem.cpp

    # Station services
    Station/StationServices.cpp

    # Manufacturing
    Manufacturing/ManufacturingQueue.cpp

    # Contract rewards
    Missions/ContractRewardSystem.cpp

    # Fleet system (D4)
    Fleet/FleetSystem.cpp

    # World simulation (D4)
    WorldSim/WorldSimSystem.cpp

    # Inventory system (D1)
    Inventory/InventorySystem.cpp

    # Storage system (D2)
    Storage/StorageSystem.cpp

    # Progression reward system (D3)
    Progression/ProgressionRewardSystem.cpp

    # Unified data registry (A3)
    Data/UnifiedDataRegistry.cpp

    # Season / Titan Race (I1)
    Season/TitanRaceSystem.cpp

    # Anomaly system (I2)
    Anomaly/AnomalySystem.cpp

    # War / Sector system (I3)
    WarSector/WarSectorSystem.cpp

    # J1/J2/J7 — Gameplay connectors
    Integration/GameplayConnector.cpp
    Integration/ContractRewardConnector.cpp
    Integration/MarketContractBridge.cpp

    # Characters (Phase 1)
    Characters/CharacterSystem.cpp
    Characters/CharacterControllerShell.cpp
    Characters/Animation/AnimationController.cpp
    Characters/Equipment/EquipmentSystem.cpp
    Characters/Mech/MechPossessionSystem.cpp

    # Characters (Phase 2)
    Characters/Core/CharacterStateAuthority.cpp
    Characters/Core/CharacterTransitionRules.cpp
    Characters/Animation/AnimationLayerSystem.cpp
    Characters/IK/IKSystem.cpp
    Characters/FPS/FPSPresentationSystem.cpp
    Characters/Editor/CharacterEditorSystem.cpp
    Characters/Tools/ToolInteractionShell.cpp
)

add_library(NovaForgeGameplay STATIC ${NOVAFORGE_GAMEPLAY_SOURCES})

target_include_directories(NovaForgeGameplay
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}"
             "${CMAKE_CURRENT_SOURCE_DIR}/Salvage"
             "${CMAKE_CURRENT_SOURCE_DIR}/Station"
             "${CMAKE_CURRENT_SOURCE_DIR}/Manufacturing"
             "${CMAKE_CURRENT_SOURCE_DIR}/Fleet"
             "${CMAKE_CURRENT_SOURCE_DIR}/WorldSim"
             "${CMAKE_CURRENT_SOURCE_DIR}/Inventory"
             "${CMAKE_CURRENT_SOURCE_DIR}/Storage"
             "${CMAKE_CURRENT_SOURCE_DIR}/Data"
             "${CMAKE_CURRENT_SOURCE_DIR}/Season"
             "${CMAKE_CURRENT_SOURCE_DIR}/Anomaly"
             "${CMAKE_CURRENT_SOURCE_DIR}/WarSector"
             "${CMAKE_CURRENT_SOURCE_DIR}/Integration"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(NovaForgeGameplay PUBLIC AtlasEngine)

atlastools_set_compiler_warnings(NovaForgeGameplay)

message(STATUS "NovaForgeGameplay: configured")

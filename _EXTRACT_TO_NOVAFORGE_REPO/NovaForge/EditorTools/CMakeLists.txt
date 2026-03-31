# NovaForge EditorTools — NovaForge-specific editor panels and tools
# Moved from Atlas/Editor/AssetTools/ during Phase 4 (Engine/Project Boundary Cleanup).
# These panels are NovaForge game-specific and must not live in the generic Atlas editor.

set(NOVAFORGE_EDITOR_TOOLS_SOURCES
    AssetPalettePanel.cpp
    AssetStylePanel.cpp
    CharacterSelectPanel.cpp
    ColonyManagerPanel.cpp
    CombatLogPanel.cpp
    DataBrowserPanel.cpp
    FleetFormationPanel.cpp
    GalaxyMapPanel.cpp
    GamePackagerPanel.cpp
    GenerationStylePanel.cpp
    InsuranceOverviewPanel.cpp
    LiveSceneManager.cpp
    MissionEditorPanel.cpp
    ModelImportPanel.cpp
    ModuleEditorPanel.cpp
    NPCEditorPanel.cpp
    PCGOverrideStore.cpp
    PCGPreviewPanel.cpp
    PhysicsTunerPanel.cpp
    SceneGraphPanel.cpp
    ShipArchetypePanel.cpp
    SolarSystemEditorPanel.cpp
    StationEditorPanel.cpp
    TradeRoutePanel.cpp
    ViewportPanel.cpp
)

add_library(NovaForgeEditorTools STATIC ${NOVAFORGE_EDITOR_TOOLS_SOURCES})

target_include_directories(NovaForgeEditorTools
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(NovaForgeEditorTools PUBLIC NovaForgeShared)

message(STATUS "NovaForgeEditorTools: configured")

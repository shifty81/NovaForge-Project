# NovaForge UI — runtime UI shell

set(NOVAFORGE_UI_SOURCES
    src/RuntimeUIShell.cpp
    src/Widgets/HUDLayer.cpp
    src/Widgets/InventoryScreen.cpp
    src/Widgets/ContractBoardUI.cpp
    src/Widgets/StationTerminalUI.cpp
    src/Widgets/FleetProgressionPanel.cpp
)

add_library(NovaForgeUI STATIC ${NOVAFORGE_UI_SOURCES})

target_include_directories(NovaForgeUI
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
             "${CMAKE_CURRENT_SOURCE_DIR}/include/Widgets"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(NovaForgeUI PUBLIC AtlasCore)

atlastools_set_compiler_warnings(NovaForgeUI)

message(STATUS "NovaForgeUI: configured")

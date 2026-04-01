# NovaForge App — bootstrap, session, project context

set(NOVAFORGE_APP_SOURCES
    src/NovaForgeApp_stub.cpp
    src/NovaForgeProjectContext.cpp
    src/NovaForgeSession.cpp
    src/NovaForgeBootstrap.cpp
    src/App.cpp
    src/GameOrchestrator.cpp
    src/DataRegistry.cpp
    Bootstrap/WorldBootstrap.cpp
)

add_library(NovaForgeApp STATIC ${NOVAFORGE_APP_SOURCES})

target_include_directories(NovaForgeApp
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/Bootstrap"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        # Renderer.h
        "${CMAKE_SOURCE_DIR}/Atlas/Engine/Rendering"
        # EditorShell.h
        "${CMAKE_SOURCE_DIR}/Atlas/Editor"
        # SaveManager.h
        "${CMAKE_SOURCE_DIR}/NovaForge/Save/include"
        # RuntimeUIShell.h
        "${CMAKE_SOURCE_DIR}/NovaForge/UI/include"
        # World.h
        "${CMAKE_SOURCE_DIR}/NovaForge/World"
)

target_link_libraries(NovaForgeApp PUBLIC AtlasRuntime NovaForgeWorld NovaForgeSave NovaForgeUI AtlasEditor)

# When bridge is enabled, NovaForgeApp links the integration layer so that
# NovaForgeBootstrap can start the AtlasBridgeService.
if(NOVAFORGE_ENABLE_ATLASAI_INTEGRATION AND NOVAFORGE_ENABLE_BRIDGE_SERVER)
    target_link_libraries(NovaForgeApp PRIVATE NovaForgeIntegrationAtlasAI)
    target_compile_definitions(NovaForgeApp
        PRIVATE NOVAFORGE_BRIDGE_SERVER_ENABLED=1)
    target_include_directories(NovaForgeApp
        PRIVATE
            "${CMAKE_SOURCE_DIR}/NovaForge/Integrations/AtlasAI/include"
            "${CMAKE_SOURCE_DIR}/Shared/AtlasBridgeContract/include"
    )
endif()

atlastools_set_compiler_warnings(NovaForgeApp)

# MasterRepoRuntime — standalone entry point executable
add_executable(MasterRepoRuntime src/main.cpp)

target_include_directories(MasterRepoRuntime
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(MasterRepoRuntime PRIVATE NovaForgeApp)

atlastools_set_compiler_warnings(MasterRepoRuntime)

message(STATUS "NovaForgeApp: configured")

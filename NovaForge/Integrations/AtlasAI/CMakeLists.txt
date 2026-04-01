# NovaForge AtlasAI Integration — bridge service for AI tooling

set(NOVAFORGE_ATLASAI_INTEGRATION_SOURCES
    src/AtlasBridgeService.cpp
    src/BridgeSessionManager.cpp
    src/BridgeAuditLogger.cpp
    src/LiveIngestionService.cpp
)

add_library(NovaForgeIntegrationAtlasAI STATIC
    ${NOVAFORGE_ATLASAI_INTEGRATION_SOURCES}
)

target_include_directories(NovaForgeIntegrationAtlasAI
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/Shared/AtlasBridgeContract/include"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(NovaForgeIntegrationAtlasAI
    PUBLIC
        AtlasBridgeContract
        AtlasRuntime
)

# Epic 7 / Task 7.5: propagate compile-time feature guards
if(NOVAFORGE_ENABLE_BRIDGE_SERVER)
    target_compile_definitions(NovaForgeIntegrationAtlasAI
        PUBLIC NOVAFORGE_BRIDGE_SERVER_ENABLED=1)
endif()

if(NOVAFORGE_ENABLE_AUDIT_LOGGING)
    target_compile_definitions(NovaForgeIntegrationAtlasAI
        PUBLIC NOVAFORGE_AUDIT_LOGGING_ENABLED=1)
endif()

atlastools_set_compiler_warnings(NovaForgeIntegrationAtlasAI)

message(STATUS "NovaForgeIntegrationAtlasAI: configured")

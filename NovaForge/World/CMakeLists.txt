# NovaForge World — galaxy, sectors, planets, stations, ships, encounters

set(NOVAFORGE_WORLD_SOURCES
    src/NovaForgeWorld_stub.cpp
    src/World.cpp
    # add source files here as world systems are implemented
)

add_library(NovaForgeWorld STATIC ${NOVAFORGE_WORLD_SOURCES})

target_include_directories(NovaForgeWorld
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(NovaForgeWorld PUBLIC AtlasEngine)

atlastools_set_compiler_warnings(NovaForgeWorld)

message(STATUS "NovaForgeWorld: configured")

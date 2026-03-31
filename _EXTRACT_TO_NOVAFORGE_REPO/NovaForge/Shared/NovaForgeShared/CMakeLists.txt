# NovaForgeShared — shared types and utilities used across NovaForge subsystems
# Moved from Atlas/Core/NovaForgeShared/ during Phase 4 (Engine/Project Boundary Cleanup).

set(NOVAFORGE_SHARED_SOURCES
    # Header-only shared types; add .cpp files here as needed
)

add_library(NovaForgeShared INTERFACE)

target_include_directories(NovaForgeShared
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
)

message(STATUS "NovaForgeShared: configured")

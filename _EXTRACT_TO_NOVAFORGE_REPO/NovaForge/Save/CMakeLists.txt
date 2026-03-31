# NovaForge Save — save/load management

set(NOVAFORGE_SAVE_SOURCES
    src/SaveManager.cpp
    src/SaveSystem.cpp
)

add_library(NovaForgeSave STATIC ${NOVAFORGE_SAVE_SOURCES})

target_include_directories(NovaForgeSave
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(NovaForgeSave PUBLIC AtlasCore)

atlastools_set_compiler_warnings(NovaForgeSave)

message(STATUS "NovaForgeSave: configured")

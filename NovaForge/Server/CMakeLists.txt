# NovaForge Server — gameplay, world, entity, PCG, and server-side systems

file(GLOB_RECURSE NOVAFORGE_SERVER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Config/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Data/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Debug/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entity/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Gameplay/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Input/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Integration/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/LegacyAdapters/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Modules/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/PCG/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Rendering/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Save/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Tooling/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/UI/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Voxel/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/World/*.cpp"
)

add_library(NovaForgeServer STATIC ${NOVAFORGE_SERVER_SOURCES})

target_include_directories(NovaForgeServer
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source"
)

target_link_libraries(NovaForgeServer
    PRIVATE AtlasLogger
)

atlastools_set_compiler_warnings(NovaForgeServer)

# ── CTest integration ─────────────────────────────────────────────────────────

if(BUILD_TESTING)
    file(GLOB NOVAFORGE_SERVER_TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp"
    )

    foreach(_test_src IN LISTS NOVAFORGE_SERVER_TEST_SOURCES)
        get_filename_component(_test_name "${_test_src}" NAME_WE)
        add_executable(${_test_name} "${_test_src}")
        target_include_directories(${_test_name}
            PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source"
        )
        target_link_libraries(${_test_name} PRIVATE NovaForgeServer)
        add_test(NAME ${_test_name} COMMAND ${_test_name})
    endforeach()
endif()

message(STATUS "NovaForge Server: configured")

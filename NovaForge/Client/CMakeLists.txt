# NovaForge Client — rendering, network, UI, audio, editor client-side systems

# ── Header-only external dependency interface targets ─────────────────────────

if(NOT TARGET NovaForge::nlohmann_json)
    add_library(NovaForge_nlohmann_json INTERFACE)
    add_library(NovaForge::nlohmann_json ALIAS NovaForge_nlohmann_json)
    target_include_directories(NovaForge_nlohmann_json INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/App/external"
    )
endif()

if(NOT TARGET NovaForge::stb)
    add_library(NovaForge_stb INTERFACE)
    add_library(NovaForge::stb ALIAS NovaForge_stb)
    target_include_directories(NovaForge_stb INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/App/external/stb"
    )
endif()

if(NOT TARGET NovaForge::tinygltf)
    add_library(NovaForge_tinygltf INTERFACE)
    add_library(NovaForge::tinygltf ALIAS NovaForge_tinygltf)
    target_include_directories(NovaForge_tinygltf INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/App/external/tinygltf"
    )
endif()

if(NOT TARGET NovaForge::tinyobjloader)
    add_library(NovaForge_tinyobjloader INTERFACE)
    add_library(NovaForge::tinyobjloader ALIAS NovaForge_tinyobjloader)
    target_include_directories(NovaForge_tinyobjloader INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/App/external/tinyobjloader"
    )
endif()

# ── NovaForgeClient static library ────────────────────────────────────────────

file(GLOB_RECURSE NOVAFORGE_CLIENT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/App/src/audio/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/App/src/characters/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/App/src/core/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/App/src/editor/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/App/src/network/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/App/src/rendering/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/App/src/ui/*.cpp"
)

# Exclude the standalone entry point from the library
list(FILTER NOVAFORGE_CLIENT_SOURCES EXCLUDE REGEX ".*/App/src/main\\.cpp$")

add_library(NovaForgeClient STATIC ${NOVAFORGE_CLIENT_SOURCES})

target_include_directories(NovaForgeClient
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/App/include"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/App/src"
)

target_link_libraries(NovaForgeClient
    PUBLIC
        NovaForge::nlohmann_json
        NovaForge::stb
        NovaForge::tinygltf
        NovaForge::tinyobjloader
        glm::glm
    PRIVATE
        $<$<TARGET_EXISTS:glfw>:glfw>
        $<$<TARGET_EXISTS:GLEW::GLEW>:GLEW::GLEW>
)

atlastools_set_compiler_warnings(NovaForgeClient)

# ── NovaForgeClientApp executable ─────────────────────────────────────────────

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/App/src/main.cpp")
    add_executable(NovaForgeClientApp
        "${CMAKE_CURRENT_SOURCE_DIR}/App/src/main.cpp"
    )
    set_target_properties(NovaForgeClientApp PROPERTIES OUTPUT_NAME "nova_forge_client")
    target_link_libraries(NovaForgeClientApp PRIVATE NovaForgeClient)
    atlastools_set_compiler_warnings(NovaForgeClientApp)

    # Copy shaders to the executable output directory so the renderer can find them
    add_custom_command(TARGET NovaForgeClientApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/App/shaders"
            "$<TARGET_FILE_DIR:NovaForgeClientApp>/shaders"
        COMMENT "Copying shaders to output directory"
    )
endif()

message(STATUS "NovaForge Client: configured")

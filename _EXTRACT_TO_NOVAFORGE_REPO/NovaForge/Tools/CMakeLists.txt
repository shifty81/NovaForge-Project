set(NOVAFORGE_TOOLS_SOURCES
    # Map project-specific editor/tooling files here
)

add_library(NovaForgeTools STATIC ${NOVAFORGE_TOOLS_SOURCES})

target_include_directories(NovaForgeTools
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(NovaForgeTools
    PUBLIC
        NovaForgeGameplay
        NovaForgeWorld
        AtlasEditor
)

target_compile_features(NovaForgeTools PUBLIC cxx_std_20)

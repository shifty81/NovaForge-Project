# NovaForge — game project, gameplay systems, data, content

include(NovaForgeTargets)

add_subdirectory(Gameplay)
add_subdirectory(World)
add_subdirectory(Save)
add_subdirectory(UI)

if(NOVAFORGE_ENABLE_ATLASAI_INTEGRATION)
    add_subdirectory(Integrations/AtlasAI)
endif()

# App depends on the integration layer when the bridge is enabled,
# so it must be added after Integrations/AtlasAI.
add_subdirectory(App)
add_subdirectory(Client)
add_subdirectory(Server)

message(STATUS "NovaForge: modules configured")

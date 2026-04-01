add_executable(NovaForgeIntegrationArbiterTests
    integration_smoke_test.cpp
)

target_link_libraries(NovaForgeIntegrationArbiterTests
    PRIVATE
        NovaForgeIntegrationArbiter
)

add_test(NAME NovaForgeIntegrationArbiterTests COMMAND NovaForgeIntegrationArbiterTests)

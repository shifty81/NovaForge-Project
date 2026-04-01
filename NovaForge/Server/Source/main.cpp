#include "Core/App.h"
#include "Shared/Logging/MasterLogger.h"
#include <exception>
#include <iostream>

int main()
{
    MasterLogger::Init("./Logs/server", "masterrepo_server.log");
    MR_LOG_INFO("main — NovaForge server starting");
    try
    {
        App app;
        if (!app.Initialize())
        {
            MR_LOG_FATAL("main — App::Initialize failed, exiting");
            std::cerr << "Failed to initialize NovaForge Runtime.\n";
            MasterLogger::Shutdown();
            return 1;
        }

        app.Run();
        app.Shutdown();
        MR_LOG_INFO("main — clean exit");
        return 0;
    }
    catch (const std::exception& ex)
    {
        MR_LOG_FATAL("main — unhandled exception: " << ex.what());
        std::cerr << "Unhandled exception: " << ex.what() << "\n";
        MasterLogger::Shutdown();
        return 2;
    }
}

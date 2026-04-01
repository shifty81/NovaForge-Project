#include "Core/App.h"
#include <exception>
#include <iostream>

int main()
{
    try
    {
        App app;
        if (!app.Initialize())
        {
            std::cerr << "Failed to initialize NovaForge Runtime.\n";
            return 1;
        }

        app.Run();
        app.Shutdown();
        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Unhandled exception: " << ex.what() << "\n";
        return 2;
    }
}

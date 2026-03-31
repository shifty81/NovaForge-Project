#include "App.h"
#include <exception>
#include <iostream>

int main()
{
    try
    {
        App AppInstance;
        if (!AppInstance.Initialize())
        {
            std::cerr << "Failed to initialize AtlasEditorRuntime\n";
            return 1;
        }

        AppInstance.Run();
        AppInstance.Shutdown();
        return 0;
    }
    catch (const std::exception& Ex)
    {
        std::cerr << "Unhandled exception: " << Ex.what() << "\n";
        return 2;
    }
}

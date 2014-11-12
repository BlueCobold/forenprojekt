#include "App.hpp"
#include "resources/AppConfig.hpp"
#include "gui/ErrorMessageBox.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        AppConfig configFile("Config.ini", "stash.dat");
        App app(configFile);
        app.run();
    }
    catch (std::exception& error)
    {
        #ifdef _DEBUG
            std::cout << "An error occurred: " << error.what();
        #elif WINDOWS
            ErrorMessageBox(error.what());
        #endif
        getchar();
        return 1;
    }
    return 0;
}
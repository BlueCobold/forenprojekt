#include "App.hpp"
#include "resources/AppConfig.hpp"
#include "gui/ErrorMessageBox.hpp"
#include <iostream>

#ifdef _DEBUG
    int main(int argc, char* argv[])
#else
    #include <Windows.h>
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
#endif
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
        #else
            ErrorMessageBox(error.what());
        #endif
        getchar();
        return 1;
    }
    return 0;
}

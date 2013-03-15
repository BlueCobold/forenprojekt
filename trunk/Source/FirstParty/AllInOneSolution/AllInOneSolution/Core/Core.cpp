#include "App.hpp"
#include "Config.hpp"

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
        Config configFile("Config.ini");
        App app(configFile);
        app.run();
    }
    catch (std::exception& error)
    {
        std::cout << "An error occurred: " << error.what();
        getchar();
        return 1;
    }

    return 0;
}

#include <iostream>
#include <string>
#include <fstream>
#include "App.hpp"
#include "ConfigFileLoader.hpp"

int main(int argc, char* argv[])
{

    ConfigFileLoader configFileLoader;

    configFileLoader.loadConfigFile("Config.ini");

    std::string windowTitle = "Test";
    // Create the Application
    App app(windowTitle, configFileLoader.getResolutionX(), configFileLoader.getResolutionY(), 
        configFileLoader.getBitsPerPixel(), configFileLoader.getFullScreen());

    app.run();
    
    return 0;
}


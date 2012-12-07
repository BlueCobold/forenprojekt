#include "App.hpp"
#include "Config.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        Config configFile("Config.ini");
        App app(configFile);
        app.run();
    }
    catch (std::runtime_error& error)
    {
        std::cout << "An error occurred: " << error.what();
        getchar();
        return 1;
    }

    return 0;
}

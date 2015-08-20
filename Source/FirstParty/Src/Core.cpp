
#include "App.hpp"
#include "gui/Cursor.hpp"
#include "gui/ErrorMessageBox.hpp"
#include "gui/MenuTemplate.hpp"
#include "rendering/Shader.hpp"
#include "resources/AppConfig.hpp"
#include "resources/SpriteSheet.hpp"

#include <SFML/System.hpp>

#include <iostream>

#ifndef WINDOWS
#include "MacHelper.hpp"
#endif

#ifdef IOS
int main2(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    main2(argc, argv);
}
int sfmlMain(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    std::ofstream file;
    std::streambuf* previous = nullptr;
    try
    {
#if defined(IOS) || (defined(OSX) && defined(_DEBUG))
        file.open(documentPath() + "error-log.txt");
#else
        file.open("error-log.txt");
#endif
        previous = sf::err().rdbuf(file.rdbuf());
        sf::err() << "=== Rickety Racquet starting ===" << std::endl;

        AppConfig configFile("Config.ini", "stash.dat");
        App app(configFile);
        app.run();
    }
    catch (std::exception& error)
    {
#ifdef _DEBUG
        std::cout << "An error occurred: " << error.what();
        getchar();
#elif WINDOWS
        ErrorMessageBox(error.what());
#else
        showErrorApple(error.what());
#endif
        return 1;
    }
    sf::err() << "=== Rickety Racquet closing ===" << std::endl;
    if(previous)
        sf::err().rdbuf(previous);
    if(file.is_open())
        file.close();

    return 0;
}
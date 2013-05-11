//#include <SFML/Window/Event.hpp>
//#include <SFML/System/Thread.hpp>
//#include <SFML/Graphics/RenderWindow.hpp>
//#include <SFML/Graphics/Texture.hpp>
//#include <SFML/Graphics/Sprite.hpp>
//#include <SFML/OpenGL.hpp>
//
//int main(int argc, char* argv[])
//{
//    sf::RenderWindow window(sf::VideoMode(800, 600), "access violation in destructor");
//
//    sf::Texture* tex = new sf::Texture(); 
//    sf::Sprite s;
//    bool loaded = false;
//    sf::Thread thread([&](){loaded = tex->loadFromFile("res/img/gui/Window.png"); glFlush(); s = sf::Sprite(*tex);});
//    thread.launch() ;
//
//    while ( window.isOpen() )
//    {
//        sf::Event event ;
//        while ( window.pollEvent(event) )
//        {
//            if ( event.type == sf::Event::Closed )
//                window.close() ;
//        }
//        if(loaded)
//        {
//            window.setTitle("Thread done, close now");
//            window.draw(s);
//        }
//    }
//    delete tex;
//
//    return 0; // Access violation here - in ~RenderWindow -> ~Window ->... ~WglContext -> wglDeleteContext()
//}

#include "App.hpp"
#include "resources/Config.hpp"

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

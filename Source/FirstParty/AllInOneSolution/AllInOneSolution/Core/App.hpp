#pragma once

#ifndef APP_HPP
#define APP_HPP

#include "ResourceManager.hpp"
#include "StateManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>

#include <string>

class Config;

/// This class will be used to handle the window or fullscreen
/// and to handle the rendering
class App
{
public:
    
    App(Config& configLoader);
    
    void run();
    
private:
    
    void update();
    void draw();
    
    void handleKeyboard();
    void handleEvents();
  
    void switchDisplayMode();
   
    void calculateFps();
    
private:

    sf::RenderWindow m_screen;
    sf::Clock m_clock;

    std::string m_windowTitle;
    unsigned int m_screenWidth;
    unsigned int m_screenHeight;
    unsigned int m_bitsPerPixel;
    bool m_fullscreen;
    bool m_focus;
   
    float m_fps;
    float m_frameCounter;

    ResourceManager m_resourceManager;

    sf::Text m_fpsText;

    StateManager m_stateManager;

};

#endif // APP_HPP

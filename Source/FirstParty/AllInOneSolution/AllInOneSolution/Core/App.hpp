#pragma once

#ifndef APP_HPP
#define APP_HPP

#include "StateManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

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
    unsigned int m_screenWidth, m_screenHeight, m_bitsPerPixel;
    bool m_fullscreen;
   
    float m_fps, m_frameCounter;

    sf::Font m_font;
    sf::Text m_fpsText;

    StateManager m_stateManager;

};

#endif // APP_HPP

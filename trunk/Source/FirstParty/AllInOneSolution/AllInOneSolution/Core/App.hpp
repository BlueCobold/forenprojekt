#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "StateManager.hpp"
#include "TestState.hpp"
#include "TestState2.hpp"

/// This class will be used to handle the window or fullscreen
/// and to handle the rendering
class App
{
    public:
    
    App(std::string& windowTitle, const unsigned int screenWidth, 
    	const unsigned int screenHeight, const unsigned int bitsPerPixel, const bool fullscreen);
    ~App();
    
    void run();
    
    private:
    
    void update();
    void draw();
    
    void handleKeyboard(); /// Will be used to handle the keyboard input
    void handleEvents(); /// Will be used to handle events from the window
  
    void switchDisplayMode();
   
    void calculateFps();

    std::string floatToString(float value);
    
    private:
  
    sf::RenderWindow m_screen;
    sf::Clock clock;
    
    std::string m_windowTitle;
    unsigned int m_screenWidth, m_screenHeight, m_bitsPerPixel;
    bool m_fullscreen;
   
    float m_fps, m_lastTime, m_currentTime  ;

    sf::Font m_font;
    sf::Text m_fpsText;

    StateManager m_stateManager;

};


#include "App.h"

App::App(std::string& windowTitle, const unsigned int screenWidth, const unsigned int screenHeight,
    const unsigned int bitsPerPixel, const bool fullscreen)
: m_windowTitle(windowTitle),
  m_screenWidth(screenWidth),
  m_screenHeight(screenHeight),
  m_bitsPerPixel(bitsPerPixel),
  m_fullscreen(fullscreen),
  m_lastTime(0),
  m_currentTime(0)

{
    if(fullscreen)
        m_window.create(sf::VideoMode(screenWidth, screenHeight, bitsPerPixel), windowTitle, sf::Style::Fullscreen);
    else
        m_window.create(sf::VideoMode(screenWidth, screenHeight, bitsPerPixel), windowTitle);
}


App::~App()
{
}

void App::run()
{
    while(m_window.isOpen())
    {
        update();
        draw();
    }
}

void App::update()
{
    handleEvents();
    handleKeyboard();	
}

void App::draw()
{
    m_window.clear();
    	 
    m_window.display();
}

void App::handleKeyboard()
{
    // Enter to window or fullscreen mode when Press Return+LAlt
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)))
    {
        m_fullscreen = !m_fullscreen;
        
        switchDisplayMode();
    }
}
void App::handleEvents()
{
    sf::Event event;
    
    while(m_window.pollEvent(event))
    {
        // Close the window
        if(event.type == sf::Event::Closed)
            m_window.close();
    }
}

void App::switchDisplayMode()
{
    if(m_fullscreen)
    {	
        // Switch to fullscreen
        m_window.create(sf::VideoMode(m_screenWidth, m_screenHeight, m_bitsPerPixel),
        m_windowTitle, sf::Style::Fullscreen);
        // Disable the cursor
        m_window.setMouseCursorVisible(false);
    }
    else
    {
        // Switch to window mode
        m_window.create(sf::VideoMode(m_screenWidth, m_screenHeight, m_bitsPerPixel), m_windowTitle);
        // Enable the cursor
        m_window.setMouseCursorVisible(true);
    }
}

void App::calculateFps()
{
    m_currentTime = clock.restart().asSeconds();
    m_fps = 1.f / (m_currentTime - m_lastTime);
    m_lastTime = m_currentTime;
}
#include "App.hpp"
#include "utility.hpp" // toString
#include "Config.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <sstream>
#include <utility> // move

App::App(Config& configLoader) :
    m_screenWidth(0),
    m_screenHeight(0),
    m_bitsPerPixel(0),
    m_fullscreen(false),
    m_fps(0.f),
    m_frameCounter(0.f)
{
    m_windowTitle = configLoader.get<std::string>("WindowName");
    m_screenWidth = configLoader.get<unsigned int>("ResolutionX");
    m_screenHeight = configLoader.get<unsigned int>("ResolutionY");
    m_bitsPerPixel = configLoader.get<unsigned int>("BitsPerPixel");
    m_fullscreen = configLoader.get<bool>("IsFullScreen");
    
    if(m_fullscreen)
    {
        m_screen.create(sf::VideoMode(m_screenWidth, m_screenHeight, m_bitsPerPixel), m_windowTitle, sf::Style::Fullscreen);
        // Disable the cursor
        m_screen.setMouseCursorVisible(false);
    }
    else
        m_screen.create(sf::VideoMode(m_screenWidth, m_screenHeight, m_bitsPerPixel), m_windowTitle);

    m_font.loadFromFile("res/font/visitor.ttf");

    m_fpsText.setFont(m_font);
    m_fpsText.setColor(sf::Color::Yellow);
    m_fpsText.setCharacterSize(30);
    m_fpsText.setPosition(10, 10);

    m_stateManager.push(std::move(std::unique_ptr<TestState>(new TestState(m_screen))));
}

void App::run()
{
    while(m_screen.isOpen())
    {
        update();
        draw();
    }
}

void App::update()
{
    calculateFps();
    m_fpsText.setString(utility::toString<float>(m_fps));
   
    handleEvents();
    handleKeyboard();	

    m_stateManager.update();
}

void App::draw()
{
    m_screen.clear();

    m_stateManager.draw();
    
    m_screen.draw(m_fpsText);
        
    m_screen.display();
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
    
    while(m_screen.pollEvent(event))
    {
        // Close the window
        if(event.type == sf::Event::Closed)
            m_screen.close();
    }
}

void App::switchDisplayMode()
{
    if(m_fullscreen)
    {	
        // Switch to fullscreen
        m_screen.create(sf::VideoMode(m_screenWidth, m_screenHeight, m_bitsPerPixel),
        m_windowTitle, sf::Style::Fullscreen);
        // Disable the cursor
        m_screen.setMouseCursorVisible(false);
    }
    else
    {
        // Switch to window mode
        m_screen.create(sf::VideoMode(m_screenWidth, m_screenHeight, m_bitsPerPixel), m_windowTitle);
        // Enable the cursor
        m_screen.setMouseCursorVisible(true);
    }
}

void App::calculateFps()
{
    m_frameCounter++;
    if(m_clock.getElapsedTime().asSeconds() >= 1.0f)
    {
        m_fps = (m_frameCounter / m_clock.getElapsedTime().asSeconds());
        m_clock.restart();
        m_frameCounter = 0;
    }
}

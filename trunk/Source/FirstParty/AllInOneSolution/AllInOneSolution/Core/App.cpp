#include "App.hpp"
#include "Config.hpp"
#include "utility.hpp" // toString
#include "PlayState.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <sstream>
#include <utility> // move

App::App(Config& config) :
    m_config(config),
    m_fullscreen(false),
    m_focus(true),
    m_fps(0),
    m_frameCounter(0.f)
{
    m_windowTitle = config.get<std::string>("WindowName");
    m_fullscreen = config.get<bool>("IsFullScreen");
    
    if(m_fullscreen)
    {
        m_screen.create(sf::VideoMode(m_config.get<unsigned int>("ResolutionX"), m_config.get<unsigned int>("ResolutionY"))
            , m_windowTitle, sf::Style::Fullscreen);
        // Disable the cursor
        m_screen.setMouseCursorVisible(false);
    }
    else
        m_screen.create(sf::VideoMode(m_config.get<unsigned int>("ResolutionX"), m_config.get<unsigned int>("ResolutionY"))
            , m_windowTitle);

    m_fpsText.setFont(*m_resourceManager.getFont("visitor.ttf"));
    m_fpsText.setColor(sf::Color::Blue);
    m_fpsText.setCharacterSize(30);
    m_fpsText.setPosition(10, 10);

    m_stateManager.push(std::unique_ptr<PlayState>(new PlayState(m_screen, m_resourceManager, m_config)));

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
    m_fpsText.setString(utility::toString<int>(m_fps));
   
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
    if(m_focus && (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)))
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
        else if(event.type == sf::Event::LostFocus)
            m_focus = false;
        else if(event.type == sf::Event::GainedFocus)
            m_focus = true;
        else if(event.type == sf::Event::Resized)
            onResize();
    }
}

void App::switchDisplayMode()
{
    if(m_fullscreen)
    {	
        // Switch to fullscreen
        m_screen.create(sf::VideoMode::getDesktopMode(), m_windowTitle, sf::Style::Fullscreen);
        // Disable the cursor
        m_screen.setMouseCursorVisible(false);
    }
    else
    {
        // Switch to window mode
        m_screen.create(sf::VideoMode(m_config.get<unsigned int>("ResolutionX"),
            m_config.get<unsigned int>("ResolutionY")), m_windowTitle);
        // Enable the cursor
        m_screen.setMouseCursorVisible(true);
    }
}

void App::calculateFps()
{
    m_frameCounter++;
    if(m_clock.getElapsedTime().asSeconds() >= 1.0f)
    {
        m_fps = static_cast<int>((m_frameCounter / m_clock.getElapsedTime().asSeconds()));
        m_clock.restart();
        m_frameCounter = 0;
    }
}

void App::onResize()
{
    sf::Vector2f size = static_cast<sf::Vector2f>(m_screen.getSize());

	// Minimum size
	if(size.x < 800)
		size.x = 800;
	if(size.y < 600)
		size.y = 600;

	// Apply possible size changes
	m_screen.setSize(static_cast<sf::Vector2u>(size));

	// Reset view
	m_screen.setView(sf::View(sf::FloatRect(0.f, 0.f, size.x, size.y)));
}

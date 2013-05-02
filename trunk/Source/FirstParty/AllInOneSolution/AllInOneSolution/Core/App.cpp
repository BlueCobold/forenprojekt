#include "App.hpp"
#include "Input.hpp"
#include "LoadLevelState.hpp"
#include "PauseState.hpp"
#include "PlayState.hpp"
#include "TransitionState.hpp"
#include "LevelPassState.hpp"
#include "resources/Config.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Audio/Listener.hpp>

#include <sstream>
#include <utility> // move

App::App(Config& config) :
    m_config(config),
    m_windowTitle("Rickety Racket"),
    m_fullscreen(false),
    m_focus(true),
    m_soundBuffer(nullptr),
    m_stateManager(m_screen)
{
    m_event.m_eventType = utility::Event::NoEvent;

    // Cache often used settings
    m_windowTitle = m_config.get<std::string>("WindowName");
    m_fullscreen = m_config.get<bool>("IsFullScreen");

    // Set the Volume for sf::Sound and sf::Music
    sf::Listener::setGlobalVolume(m_config.get<float>("MasterVolume"));

    sf::VideoMode videoMode(m_config.get<unsigned int>("ResolutionX"), m_config.get<unsigned int>("ResolutionY"));

    adjustVideoMode(videoMode);

    if(m_fullscreen)
    {
        m_screen.create(videoMode, m_windowTitle, sf::Style::Fullscreen);
        // Disable the cursor
        m_screen.setMouseCursorVisible(false);
    }
    else
        m_screen.create(videoMode, m_windowTitle);

    m_screen.setFramerateLimit(m_config.get<int>("FrameRateLimit"));
    m_screen.setVerticalSyncEnabled(m_config.get<bool>("Vsync"));

    m_stateManager.registerState(LoadLevelStateId, std::unique_ptr<LoadLevelState>(new LoadLevelState(m_screen, m_resourceManager, m_config, m_event))); 
    m_stateManager.registerState(PlayStateId, std::unique_ptr<PlayState>(new PlayState(m_screen, m_resourceManager, m_config, m_event))); 
    m_stateManager.registerState(PauseStateId, std::unique_ptr<PauseState>(new PauseState(m_screen, m_resourceManager, m_config, m_event)));
    m_stateManager.registerState(TransitionStateId, std::unique_ptr<TransitionState>(new TransitionState(m_screen, m_resourceManager, m_config, m_event)));
    m_stateManager.registerState(LevelPassStateId, std::unique_ptr<LevelPassState>(new LevelPassState(m_screen, m_resourceManager, m_config, m_event)));
    m_stateManager.setState(LoadLevelStateId);
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
    sf::View view(sf::FloatRect(0.f, 0.f,
        static_cast<float>(m_screen.getSize().x),
        static_cast<float>(m_screen.getSize().y)));
    m_screen.setView(view);

    m_stateManager.update();

    utility::Mouse.capture();

    handleEvents();
    handleKeyboard();
}

void App::draw()
{
#ifdef _DEBUG
    if(utility::Keyboard.isKeyPressed(sf::Keyboard::Z))
        m_screen.clear();
#endif

    m_stateManager.draw();

    m_screen.display();
}

void App::handleKeyboard()
{
    // Enter to window or fullscreen mode when Press Return+LAlt
    if(m_focus && (utility::Keyboard.isKeyDown(sf::Keyboard::Return) && utility::Keyboard.isKeyPressed(sf::Keyboard::LAlt)))
    {
        m_fullscreen = !m_fullscreen;
        
        switchDisplayMode();
    }
}

void App::handleEvents()
{
    sf::Event event;

    utility::Keyboard.progress();
    while(m_screen.pollEvent(event))
    {
        m_event.m_eventType = utility::Event::NoEvent;
        // Close the window
        if(event.type == sf::Event::Closed)
            m_screen.close();
        else if(event.type == sf::Event::LostFocus)
        {
            m_event.m_eventType = utility::Event::LostFocus;
            m_focus = false;
        }
        else if(event.type == sf::Event::GainedFocus)
        {
            m_event.m_eventType = utility::Event::GainFocus;
            m_focus = true;
        }
        else if(event.type == sf::Event::Resized)
        {
            m_event.m_eventType = utility::Event::Resized;
            onResize();
        }
        else if(event.type == sf::Event::KeyPressed)
            utility::Keyboard.notifyKeyPressed(event.key.code);
        else if(event.type == sf::Event::KeyReleased)
            utility::Keyboard.notifyKeyReleased(event.key.code);
    }
}

void App::switchDisplayMode()
{
    sf::VideoMode videoMode(m_config.get<unsigned int>("ResolutionX"), m_config.get<unsigned int>("ResolutionY"));

    adjustVideoMode(videoMode);

    if(m_fullscreen)
    {
        // Switch to fullscreen
        m_screen.create(sf::VideoMode(videoMode), m_windowTitle, sf::Style::Fullscreen);
        // Disable the cursor
        m_screen.setMouseCursorVisible(false);
    }
    else
    {
        // Switch to window mode
        m_screen.create(sf::VideoMode(videoMode), m_windowTitle);
        // Enable the cursor
        m_screen.setMouseCursorVisible(true);
    }
    m_screen.setFramerateLimit(m_config.get<int>("FrameRateLimit"));
    m_screen.setVerticalSyncEnabled(m_config.get<bool>("Vsync"));
}

void App::onResize()
{
    sf::VideoMode mode(m_screen.getSize().x, m_screen.getSize().y);

    adjustVideoMode(mode);
    
    // Apply possible size changes
    m_screen.setSize(sf::Vector2u(mode.width, mode.height));
}

void App::adjustVideoMode(sf::VideoMode& mode)
{
    // Ensure minimas and maximas
    if(mode.width > 1920)
        mode.width = 1920;
    else if(mode.width < 800)
        mode.width = 800;

    if(mode.height > 1080)
        mode.height = 1080;
    else if(mode.height < 600)
        mode.height = 600;

    // Validity of resolution is only important in fullscreen mode
    if(m_fullscreen && !mode.isValid())
    {
        // Get sorted fullscreen modes - best before worse
        const std::vector<sf::VideoMode>& modes = sf::VideoMode::getFullscreenModes();

        for(std::size_t i = 1; i < modes.size(); ++i)
        {
            if(modes[i].width < mode.width && modes[i].height < mode.height &&
                modes[i-1].width <= 1920 && modes[i-1].height <= 1080)
            {
                mode = modes[i-1];
                break;
            }
        }
    }
    else if(!m_fullscreen)
    {
        if(mode.width > sf::VideoMode::getDesktopMode().width)
            mode.width = sf::VideoMode::getDesktopMode().width;
        if(mode.height > sf::VideoMode::getDesktopMode().height)
            mode.height = sf::VideoMode::getDesktopMode().height;
    }
}
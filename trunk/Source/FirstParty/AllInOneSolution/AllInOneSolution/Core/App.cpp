#include "App.hpp"
#include "Input.hpp"
#include "../Core/State/LoadLevelState.hpp"
#include "../Core/State/PauseState.hpp"
#include "../Core/State/PlayState.hpp"
#include "../Core/State/TransitionState.hpp"
#include "../Core/State/LevelPassState.hpp"
#include "../Core/State/LevelFailState.hpp"
#include "../Core/State/MainMenuState.hpp"
#include "../Core/State/StartState.hpp"
#include "../Core/State/OptionMenuState.hpp"
#include "../Core/State/CreditMenuState.hpp"
#include "../Core/State/LevelPreviewState.hpp"
#include "../Core/State/LevelSelectState.hpp"
#include "../Core/State/CoinShopState.hpp"
#include "resources/AppConfig.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Audio/Listener.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <sstream>
#include <utility> // move

App::App(AppConfig& config) :
    m_config(config),
    m_windowTitle("Rickety Racquet"),
    m_fullscreen(false),
    m_focus(true),
    m_stateManager(m_screen)
{
    // Cache often used settings
    m_windowTitle = m_config.get<std::string>("WindowName");
    m_fullscreen = m_config.get<bool>("IsFullScreen");
    sf::VideoMode videoMode(m_config.get<unsigned int>("ResolutionX"), m_config.get<unsigned int>("ResolutionY"));

    adjustVideoMode(videoMode);

    if(m_fullscreen)
    {
        m_screen.create(videoMode, m_windowTitle, sf::Style::Fullscreen);
        // Disable the cursor
        //m_screen.setMouseCursorVisible(false);
    }
    else
        m_screen.create(videoMode, m_windowTitle);

    m_screen.setMouseCursorVisible(false);
    m_cursor = std::unique_ptr<Cursor>(new Cursor(m_resourceManager, m_screen));

    sf::Image icon; icon.create(16, 16);
    sf::Image texture = m_resourceManager.getTexture("GuiElements")->copyToImage();
    icon.copy(texture, 0, 0, sf::IntRect(1000, 20, 16, 16));
    m_screen.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    m_screen.setFramerateLimit(m_config.get<int>("FrameRateLimit"));
    m_screen.setVerticalSyncEnabled(m_config.get<bool>("Vsync"));

    m_stateManager.registerState(LoadLevelStateId, std::unique_ptr<LoadLevelState>(new LoadLevelState(m_screen, m_resourceManager, m_config))); 
    m_stateManager.registerState(PlayStateId, std::unique_ptr<PlayState>(new PlayState(m_screen, m_resourceManager, m_config))); 
    m_stateManager.registerState(PauseStateId, std::unique_ptr<PauseState>(new PauseState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(TransitionStateId, std::unique_ptr<TransitionState>(new TransitionState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(LevelPassStateId, std::unique_ptr<LevelPassState>(new LevelPassState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(LevelFailStateId, std::unique_ptr<LevelFailState>(new LevelFailState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(MainMenuStateId, std::unique_ptr<MainMenuState>(new MainMenuState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(StartStateId, std::unique_ptr<StartState>(new StartState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(OptionMenuStateId, std::unique_ptr<OptionMenuState>(new OptionMenuState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(CreditMenuStateId, std::unique_ptr<CreditMenuState>(new CreditMenuState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(LevelPreviewStateId, std::unique_ptr<LevelPreviewState>(new LevelPreviewState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(LevelSelectStateId, std::unique_ptr<LevelSelectState>(new LevelSelectState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(CoinShopStateId, std::unique_ptr<CoinShopState>(new CoinShopState(m_screen, m_resourceManager, m_config)));
    m_stateManager.setState(StartStateId);
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

    handleEvents();
    handleKeyboard();

    m_resourceManager.getSoundManager().update();
    m_stateManager.passEvent(m_event.m_eventType);
    m_stateManager.update();

    utility::Mouse.capture();
    m_cursor->update();
}

void App::draw()
{
#ifdef _DEBUG
    if(utility::Keyboard.isKeyPressed(sf::Keyboard::Z))
        m_screen.clear();
#endif

    m_stateManager.draw();
    m_cursor->draw(m_screen);

    m_screen.display();
}

void App::handleKeyboard()
{
    // Enter to window or fullscreen mode when Press Return+LAlt
    if(m_focus && (utility::Keyboard.isKeyDown(sf::Keyboard::Return) && utility::Keyboard.isKeyPressed(sf::Keyboard::LAlt)))
    {
        m_fullscreen = !m_config.get<bool>("IsFullScreen");
        m_config.set("IsFullScreen", m_fullscreen);
        switchDisplayMode();
    }
}

void App::handleEvents()
{
    sf::Event event;
    m_event.m_eventType = utility::Event::NoEvent;

    utility::Keyboard.progress();
    while(m_screen.pollEvent(event))
    {
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
        else if(event.type == sf::Event::MouseWheelMoved)
            utility::Mouse.notifyWheelMoved(event.mouseWheel.delta);
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
        //m_screen.setMouseCursorVisible(false);
        m_event.m_eventType = utility::Event::Resized;
    }
    else
    {
        // Switch to window mode
        m_screen.create(sf::VideoMode(videoMode), m_windowTitle);
        // Enable the cursor
        m_screen.setMouseCursorVisible(true);
        m_event.m_eventType = utility::Event::Resized;
    }
    m_screen.setMouseCursorVisible(false);
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

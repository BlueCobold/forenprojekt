
#include "App.hpp"

#include "Input.hpp"
#include "gui/Cursor.hpp"
#include "State/LoadLevelState.hpp"
#include "State/PauseState.hpp"
#include "State/PlayState.hpp"
#include "State/TransitionState.hpp"
#include "State/LevelPassState.hpp"
#include "State/LevelFailState.hpp"
#include "State/MainMenuState.hpp"
#include "State/StartState.hpp"
#include "State/OptionMenuState.hpp"
#include "State/CreditMenuState.hpp"
#include "State/LevelPreviewState.hpp"
#include "State/LevelSelectState.hpp"
#include "State/CoinShopState.hpp"
#include "State/HighScoreState.hpp"
#include "State/NewHighScoreState.hpp"
#include "State/GameFinishedState.hpp"
#include "State/AchievementState.hpp"
#include "rendering/GLExt.hpp"
#include "rendering/Shader.hpp"
#include "resources/AppConfig.hpp"
#include "resources/SpriteSheet.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Audio/Listener.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderOptions.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>

#include <map>
#include <sstream>
#include <utility> // move, make_pair

#ifndef WINDOWS
#import "MacHelper.hpp"
#endif

App::App(AppConfig& config) :
    m_config(config),
    m_windowTitle("Rickety Racquet"),
    m_fullscreen(false),
    m_focus(true),
    m_isMinimized(false),
    m_achievementManager("Achievement.dat", m_config),
    m_resourceManager(*this)
{
    gl::sys::LoadFunctions();
    int maxTextureSize = 0;
    gl::GetIntegerv(gl::MAX_TEXTURE_SIZE, &maxTextureSize);
    sf::err() << "Max texure size: " << maxTextureSize << std::endl;
    sf::err() << "GLVersion: " << gl::sys::GetMajorVersion() << "." << gl::sys::GetMinorVersion() << std::endl;
    m_clock.restart();

    // Cache often used settings
    sfExt::StencilBufferEnabled = m_config.get<bool>("UseStencilEffects");
    m_windowTitle = m_config.get<std::string>("WindowName");
    m_fullscreen = m_config.get<bool>("IsFullScreen");
#ifdef IOS
    auto videoMode = sf::VideoMode::getDesktopMode();
    if(videoMode.width < videoMode.height) {
        std::swap(videoMode.width, videoMode.height);
    }
#else
    sf::VideoMode videoMode(m_config.get<unsigned int>("ResolutionX"), m_config.get<unsigned int>("ResolutionY"));
#endif

    adjustVideoMode(videoMode);

    sf::ContextSettings settings = sf::ContextSettings(24, 8, 0);
    m_screen.create(videoMode, m_windowTitle, m_fullscreen ? sf::Style::Fullscreen : sf::Style::Default, settings);

    // This kind of redundant line prevents shader-renderings to show the first frame incorrectly
    m_screen.resetGLStates();

    // This kind of redundant line prevents a thread-based crash when closing the app.
    // It is based on a known SFML bug: https://github.com/LaurentGomila/SFML/issues/790
    sf::Shader::isAvailable();
    
    auto desktopMode = sf::VideoMode::getDesktopMode();
    auto size = static_cast<int>(desktopMode.width > desktopMode.height ?
        desktopMode.width : desktopMode.height);
    auto powerOfTwo = static_cast<int>(powf(2.f,
        floorf(logf(static_cast<float>(size))/logf(2.f))));
    if(powerOfTwo < size)
        powerOfTwo *= 2;

    for(unsigned int i = 0; i < m_offscreens.size(); i++)
    {
        m_offscreens[i].create(powerOfTwo, powerOfTwo, true);
        m_resourceManager.addTexture("offscreenBuffer" + utility::toString(i + 1), m_offscreens[i].getTexture());
    }

    m_cursor = std::unique_ptr<Cursor>(new Cursor(m_resourceManager, m_screen));

    auto sheet = m_resourceManager.getSpriteSheet("gui_elements");
    auto iconSprite = sheet->get("Icon_16");
    sf::Image icon;
    icon.create(iconSprite.width, iconSprite.height);
    sf::Image texture = m_resourceManager.getTexture(sheet->getTextureName())->copyToImage();
    icon.copy(texture, 0, 0, sf::IntRect(iconSprite.x, iconSprite.y, iconSprite.width, iconSprite.height));
    m_screen.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    m_screen.setFramerateLimit(m_config.get<int>("FrameRateLimit"));
    m_screen.setVerticalSyncEnabled(m_config.get<bool>("Vsync"));

    m_stateManager.registerState(LoadLevelStateId, std::unique_ptr<LoadLevelState>(new LoadLevelState(m_screen, m_resourceManager, m_config))); 
    m_stateManager.registerState(PlayStateId, std::unique_ptr<PlayState>(new PlayState(m_screen, m_resourceManager, m_config))); 
    m_stateManager.registerState(PauseStateId, std::unique_ptr<PauseState>(new PauseState(m_screen, m_resourceManager, m_config)));
    std::vector<sf::RenderTexture*> buffers;
    for(unsigned int i = 0; i < m_offscreens.size(); i++)
        buffers.push_back(&m_offscreens[i]);
    m_stateManager.registerState(TransitionStateId, std::unique_ptr<TransitionState>(new TransitionState(m_screen, m_resourceManager, m_config, buffers)));
    m_stateManager.registerState(LevelPassStateId, std::unique_ptr<LevelPassState>(new LevelPassState(m_screen, m_resourceManager, m_config, m_achievementManager)));
    m_stateManager.registerState(LevelFailStateId, std::unique_ptr<LevelFailState>(new LevelFailState(m_screen, m_resourceManager, m_config, m_achievementManager)));
    m_stateManager.registerState(MainMenuStateId, std::unique_ptr<MainMenuState>(new MainMenuState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(StartStateId, std::unique_ptr<StartState>(new StartState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(OptionMenuStateId, std::unique_ptr<OptionMenuState>(new OptionMenuState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(CreditMenuStateId, std::unique_ptr<CreditMenuState>(new CreditMenuState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(LevelPreviewStateId, std::unique_ptr<LevelPreviewState>(new LevelPreviewState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(LevelSelectStateId, std::unique_ptr<LevelSelectState>(new LevelSelectState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(CoinShopStateId, std::unique_ptr<CoinShopState>(new CoinShopState(m_screen, m_resourceManager, m_config, m_achievementManager)));
    m_stateManager.registerState(HighScoreStateId, std::unique_ptr<HighScoreState>(new HighScoreState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(NewHighScoreStateId, std::unique_ptr<NewHighScoreState>(new NewHighScoreState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(GameFinishedStateId, std::unique_ptr<GameFinishedState>(new GameFinishedState(m_screen, m_resourceManager, m_config)));
    m_stateManager.registerState(AchievementStateId, std::unique_ptr<AchievementState>(new AchievementState(m_screen, m_resourceManager, m_config, m_achievementManager)));
    m_stateManager.setState(StartStateId);
    
    m_screen.setMouseCursorVisible(false);
#ifdef IOS
    utility::Mouse.enableSensors(true);
#endif
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
    updateCurrentTime(m_clock.getElapsedTime().asSeconds());

    ResourceManager::ShaderContext* context = this;

    sf::View view(sf::FloatRect(0.f, 0.f,
        static_cast<float>(m_screen.getSize().x),
        static_cast<float>(m_screen.getSize().y)));
    m_screen.setView(view);

    handleEvents();
    handleKeyboard();

    m_resourceManager.getSoundManager().update();
    m_stateManager.update();

    if(!m_isMinimized)
        utility::Mouse.capture();
    m_cursor->update();
}

void App::draw()
{
#if IOS
    if(!m_focus)
    {
        sf::sleep(sf::milliseconds(10));
        return;
    }
#endif

#ifdef _DEBUG
    if(utility::Keyboard.isKeyPressed(sf::Keyboard::Z))
        m_screen.clear();
#endif
    gl::StencilMask(0xFF); // Write to entire stencil buffer
    gl::ClearStencil(0);
    gl::Clear(gl::STENCIL_BUFFER_BIT);

    auto params = DrawParameter(m_screen);
    std::unordered_map<const sf::Texture*, sf::RenderTexture*> offscreens;
    for(unsigned int i = 0; i < m_offscreens.size(); i++)
    {
        params.addTargetBuffer(m_offscreens[i]);
        m_offscreens[i].clear(sf::Color(0x7F7F7F00));
        offscreens.insert(std::make_pair(&(m_offscreens[i].getTexture()), &m_offscreens[i]));
    }

    params.registerBufferRequest([&](const sf::Texture* texture)
    {
        auto pair = offscreens.find(texture);
        if(pair != end(offscreens))
        {
            pair->second->display();
            offscreens.erase(texture);
        }
    });

    m_stateManager.draw(params);
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
    // Strg + S = will enable or disable stencil shadows at any time
    if((utility::Keyboard.isKeyDown(sf::Keyboard::LControl) && utility::Keyboard.isKeyPressed(sf::Keyboard::S)) ||
       (utility::Keyboard.isKeyPressed(sf::Keyboard::LControl) && utility::Keyboard.isKeyDown(sf::Keyboard::S)))
    {
        Animation::enableStencilEffects(!Animation::usesStencilEffects());
        bool stencilEffects = m_config.get<bool>("UseStencilEffects");
        m_config.set<bool>("UseStencilEffects", !stencilEffects);
    }
    // Q = will minimize the game to the task bar and pause an eventually running level.
    if(utility::Keyboard.isKeyReleased(sf::Keyboard::Q))
    {
        minimize();
        // notify the state manager to pause all states and prevent messing arround with
        // keyboard or mouse (PlayState for example) - waiting for FocusLost would be too late
        m_stateManager.passEvent(utility::Event::LostFocus);
    }
}

void App::handleEvents()
{
    sf::Event event;
    m_event.m_eventType = utility::Event::NoEvent;

    utility::Keyboard.progress();
    while(m_screen.isOpen() && m_screen.pollEvent(event))
    {
        // Close the window
        if(event.type == sf::Event::Closed)
            m_screen.close();
        else if(event.type == sf::Event::LostFocus)
        {
            m_event.m_eventType = utility::Event::LostFocus;
            m_focus = false;
#if IOS
            // The app might never return due to the user closing the app which causes
            // a sigkill and thus destructors will not be called.
            // So better save now than losing everything in the process.
            m_config.save();
#endif
        }
        else if(event.type == sf::Event::GainedFocus)
        {
            m_event.m_eventType = utility::Event::GainFocus;
            m_focus = true;
            if(m_isMinimized)
                restore();
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
        else if(event.type == sf::Event::MouseButtonPressed)
            utility::Mouse.notifyButtonPressed(event.mouseButton.button);
        else if(event.type == sf::Event::MouseButtonReleased)
            utility::Mouse.notifyButtonReleased(event.mouseButton.button);
#ifdef IOS
        else if(event.type == sf::Event::TouchBegan)
        {
            utility::Mouse.notifyButtonPressed(event.mouseButton.button);
            utility::Mouse.notifyTouch(sf::Vector2i(event.touch.x, event.touch.y));
        }
        else if(event.type == sf::Event::TouchEnded)
        {
            utility::Mouse.notifyButtonReleased(event.mouseButton.button);
            utility::Mouse.notifyTouch(sf::Vector2i(event.touch.x, event.touch.y));
        }
        else if(event.type == sf::Event::TouchMoved)
            utility::Mouse.notifyTouch(sf::Vector2i(event.touch.x, event.touch.y));
#endif
        
        m_stateManager.passEvent(m_event.m_eventType);
    }
}

void App::switchDisplayMode()
{
    sf::VideoMode videoMode(m_config.get<unsigned int>("ResolutionX"), m_config.get<unsigned int>("ResolutionY"));

    adjustVideoMode(videoMode);

    sf::ContextSettings settings = sf::ContextSettings(24, 8, 0);
    if(m_fullscreen)
    {
        // Switch to fullscreen
        m_screen.create(sf::VideoMode(videoMode), m_windowTitle, sf::Style::Fullscreen, settings);
        // Disable the cursor
        //m_screen.setMouseCursorVisible(false);
        m_event.m_eventType = utility::Event::Resized;
    }
    else
    {
        // Switch to window mode
        m_screen.create(sf::VideoMode(videoMode), m_windowTitle, sf::Style::Default, settings);
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
#ifndef IOS
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
    m_config.set("ResolutionX", mode.width);
    m_config.set("ResolutionY", mode.height);
#endif
}

void App::minimize()
{
    m_isMinimized = true;
    if(m_config.get<int>("IsFullScreen"))
    {
        auto desktop = sf::VideoMode::getDesktopMode();
        if(desktop.width != m_config.get<unsigned int>("ResolutionX") && desktop.height != m_config.get<unsigned int>("ResolutionY"))
        {
            auto mode = sf::VideoMode::getDesktopMode();
            adjustVideoMode(mode);
            m_screen.create(mode, m_windowTitle, sf::Style::Default, sf::ContextSettings(24, 8, 0));
        }
    }
#ifdef WINDOWS
    ShowWindow(m_screen.getSystemHandle(), SW_MINIMIZE);
#else
    ::minimize(m_screen.getSystemHandle());
#endif
}

void App::restore()
{
    m_isMinimized = false;
    if(m_config.get<int>("IsFullScreen"))
    {
        auto mode = sf::VideoMode(m_config.get<unsigned int>("ResolutionX"), m_config.get<unsigned int>("ResolutionY"));
        adjustVideoMode(mode);
        if(mode.width != m_config.get<unsigned int>("ResolutionX") && mode.height != m_config.get<unsigned int>("ResolutionY"))
            m_screen.create(mode, m_windowTitle, sf::Style::Fullscreen, sf::ContextSettings(24, 8, 0));
        m_screen.setMouseCursorVisible(false);
        m_screen.setFramerateLimit(m_config.get<int>("FrameRateLimit"));
        m_screen.setVerticalSyncEnabled(m_config.get<bool>("Vsync"));
    }
#ifdef WINDOWS
    ShowWindow(m_screen.getSystemHandle(), SW_RESTORE);
#else
    ::maximize(m_screen.getSystemHandle());
#endif
}

float App::getValueOf(const std::string& name) const
{
    auto found = m_variables.find(name);
    if(found != end(m_variables))
        return found->second;
    else
        throw std::runtime_error(utility::replace(utility::translateKey("NoVariable"), name));
    return 0;
}

void App::setValueOf(const std::string& name, const float value)
{
    m_variables[name] = value;
}


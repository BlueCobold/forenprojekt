#include "OptionMenu.hpp"

#include "Slider.hpp"
#include "../resources/AppConfig.hpp"
#include "../resources/ResourceManager.hpp"
#include "../animation/Animation.hpp"
#include <SFML/Audio/Listener.hpp>

template<class T>
bool compareVector2 (const sf::Vector2<T> lhs, const sf::Vector2<T> rhs);

OptionMenu::OptionMenu(const sf::Vector2f& position,
                       sf::RenderWindow& screen,
                       ResourceManager& resourceManager,
                       AppConfig& config) :
    CaptionMenu(*resourceManager.getMenuTemplate("OptionMenu"), position, screen),
    m_config(config),
    m_fullScreen(false),
    m_currentVideoModeIndex(0)
{
    m_fullScreen = m_config.get<bool>("IsFullScreen");

    m_muteSoundWhenInactive = m_config.get<bool>("MuteSoundWhenInactiv");

    m_masterVolume = m_config.get<float>("MasterVolume");

    m_useVerticalAxis = m_config.get<bool>("UseVerticalAxis");

    m_invertAxis = m_config.get<bool>("InvertAxis");

    m_useStencilEffects = m_config.get<bool>("UseStencilEffects");

    sf::Listener::setGlobalVolume(m_masterVolume);

    Menu::getCheckbox(CHECKBOX_FULLSCREEN).setChecked(m_fullScreen);

    Menu::getSlider(SLIDER_MASTERVOLUMEN).setValue(static_cast<float>(m_masterVolume));

    Menu::getCheckbox(CHECKBOX_MUTEINACTIVE).setChecked(m_muteSoundWhenInactive);

    Menu::getCheckbox(CHECKBOX_INVERT_AXIS).setChecked(m_invertAxis);

    Menu::getCheckbox(CHECKBOX_USE_VERTICALAXIS).setChecked(m_useVerticalAxis);

    Menu::getCheckbox(CHECKBOX_USE_STENCIL_EFFECTS).setChecked(m_useStencilEffects);
    Animation::enableStencilEffects(m_useStencilEffects);

    for(auto it = begin(sf::VideoMode::getFullscreenModes()); it != end(sf::VideoMode::getFullscreenModes()); ++it)
    {
        if(acceptableVideoMode(*it) && it->bitsPerPixel == 32)
            m_availableVideoMode.push_back(sf::Vector2u(it->width, it->height));
    }

    m_icon.create(16, 16);
    sf::Image texture = resourceManager.getTexture("GuiElements")->copyToImage();
    m_icon.copy(texture, 0, 0, sf::IntRect(1000, 20, 16, 16));
}

void OptionMenu::applyChanges()
{
    if(Menu::getCheckbox(CHECKBOX_FULLSCREEN).getChecked() != m_fullScreen ||
       m_currentVideoMode.x != m_config.get<unsigned int>("ResolutionX") ||
       m_currentVideoMode.y != m_config.get<unsigned int>("ResolutionY"))
    {
        if(Menu::getCheckbox(CHECKBOX_FULLSCREEN).getChecked() != m_fullScreen)
            m_fullScreen = !m_fullScreen;

        sf::VideoMode videoMode(m_currentVideoMode.x, m_currentVideoMode.y);
        adjustVideoMode(videoMode, m_fullScreen);

        sf::ContextSettings settings = sf::ContextSettings(24, 8, 0);
        if(m_fullScreen)
            Menu::getRenderWindow().create(videoMode, utility::translateKey("gui_rickety_racquet"), sf::Style::Fullscreen, settings);
        if(!m_fullScreen)
        {
            Menu::getRenderWindow().create(sf::VideoMode(videoMode), utility::translateKey("gui_rickety_racquet"), sf::Style::Default, settings); 
            Menu::getRenderWindow().setIcon(m_icon.getSize().x, m_icon.getSize().y, m_icon.getPixelsPtr());
        }
        
        if(!sf::VideoMode(m_appointedVideoMode.x, m_appointedVideoMode.y).isValid())
        {
            auto index = std::find(begin(m_availableVideoMode), end(m_availableVideoMode), m_appointedVideoMode);
            if(index != end(m_availableVideoMode))
                m_availableVideoMode.erase(index);
        }

        Menu::getRenderWindow().setMouseCursorVisible(false);

        m_config.set("IsFullScreen", m_fullScreen);
        m_config.set("ResolutionX", m_currentVideoMode.x);
        m_config.set("ResolutionY", m_currentVideoMode.y);

        Menu::getRenderWindow().setFramerateLimit(m_config.get<int>("FrameRateLimit"));
        Menu::getRenderWindow().setVerticalSyncEnabled(m_config.get<bool>("Vsync"));
    }

    if(Menu::getSlider(SLIDER_MASTERVOLUMEN).getValue() != m_masterVolume)
    {
        m_masterVolume = Menu::getSlider(SLIDER_MASTERVOLUMEN).getValue();
        sf::Listener::setGlobalVolume(m_masterVolume);
        m_config.set("MasterVolume", m_masterVolume);
    }

    if(Menu::getCheckbox(CHECKBOX_MUTEINACTIVE).getChecked() != m_muteSoundWhenInactive)
    {
        m_muteSoundWhenInactive = !m_muteSoundWhenInactive;
        m_config.set("MuteSoundWhenInactiv", m_muteSoundWhenInactive);
    }

    if(Menu::getCheckbox(CHECKBOX_USE_VERTICALAXIS).getChecked() != m_useVerticalAxis)
    {
        m_useVerticalAxis = !m_useVerticalAxis;
        m_config.set("UseVerticalAxis", m_useVerticalAxis);
    }

    if(Menu::getCheckbox(CHECKBOX_INVERT_AXIS).getChecked() != m_invertAxis)
    {
        m_invertAxis = !m_invertAxis;
        m_config.set("InvertAxis", m_invertAxis);
    }

    if(Menu::getCheckbox(CHECKBOX_USE_STENCIL_EFFECTS).getChecked() != m_useStencilEffects)
    {
        m_useStencilEffects = !m_useStencilEffects;
        m_config.set("UseStencilEffects", m_useStencilEffects);
        Animation::enableStencilEffects(m_useStencilEffects);
    }
}

void OptionMenu::adjustVideoMode(sf::VideoMode& mode, bool fullScreen)
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
    if(fullScreen && !mode.isValid())
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
    else if(!fullScreen)
    {
        if(mode.width > sf::VideoMode::getDesktopMode().width)
            mode.width = sf::VideoMode::getDesktopMode().width;
        if(mode.height > sf::VideoMode::getDesktopMode().height)
            mode.height = sf::VideoMode::getDesktopMode().height;
    }
}

void OptionMenu::onEnter()
{
    m_fullScreen = m_config.get<bool>("IsFullScreen");

    m_muteSoundWhenInactive = m_config.get<bool>("MuteSoundWhenInactiv");

    m_masterVolume = m_config.get<float>("MasterVolume");
    sf::Listener::setGlobalVolume(m_masterVolume);

    m_invertAxis = m_config.get<bool>("InvertAxis");
    m_useVerticalAxis = m_config.get<bool>("UseVerticalAxis");

    Menu::getCheckbox(CHECKBOX_FULLSCREEN).setChecked(m_fullScreen);

    Menu::getSlider(SLIDER_MASTERVOLUMEN).setValue(static_cast<float>(m_masterVolume));

    Menu::getCheckbox(CHECKBOX_MUTEINACTIVE).setChecked(m_muteSoundWhenInactive);

    Menu::getCheckbox(CHECKBOX_USE_VERTICALAXIS).setChecked(m_useVerticalAxis);

    Menu::getCheckbox(CHECKBOX_INVERT_AXIS).setChecked(m_invertAxis);

    // necessary becaus onEnter() is called twice
    auto screenSize = getRenderWindow().getSize();
    if(!sf::VideoMode(screenSize.x, screenSize.y).isValid() &&
       std::find(begin(m_availableVideoMode), end(m_availableVideoMode), screenSize) == end(m_availableVideoMode))
        m_availableVideoMode.push_back(screenSize);

    m_currentVideoMode = screenSize;
    m_appointedVideoMode = m_currentVideoMode;

    sortVideoModeList();
}

void OptionMenu::nextVideoMode()
{
    m_currentVideoModeIndex++;
    if(static_cast<unsigned int>(m_currentVideoModeIndex) >= m_availableVideoMode.size())
        m_currentVideoModeIndex = 0;

    m_currentVideoMode = m_availableVideoMode[m_currentVideoModeIndex];

    if(m_appointedVideoMode == m_currentVideoMode)
        getLabel(LABEL_RESOLUTION).setText("Current");
    else
        getLabel(LABEL_RESOLUTION).setText(utility::toString(m_currentVideoMode.x) + 
                                           utility::toString(" x ") + 
                                           utility::toString(m_currentVideoMode.y));
}

void OptionMenu::prevVideoMode()
{
    m_currentVideoModeIndex--;
    if(m_currentVideoModeIndex < 0)
        m_currentVideoModeIndex = m_availableVideoMode.size() - 1;

    m_currentVideoMode = m_availableVideoMode[m_currentVideoModeIndex];

    if(m_appointedVideoMode == m_currentVideoMode)
        getLabel(LABEL_RESOLUTION).setText("Current");
    else
        getLabel(LABEL_RESOLUTION).setText(utility::toString(m_currentVideoMode.x) + 
                                           utility::toString(" x ") + 
                                           utility::toString(m_currentVideoMode.y));
}

bool OptionMenu::acceptableVideoMode(const sf::VideoMode videoMode)
{
    if(videoMode.width > 1920 || videoMode.width < 800 || videoMode.height > 1080 || videoMode.height < 600)
        return false;

    return true;
}

void OptionMenu::sortVideoModeList()
{
    std::sort(begin(m_availableVideoMode), end(m_availableVideoMode), compareVector2<unsigned int>);

    for(unsigned int i = 0; i < m_availableVideoMode.size(); ++i)
        if(m_currentVideoMode == m_availableVideoMode[i])
        {
            m_currentVideoModeIndex = i;
            break;
        }
}

template<class T>
bool compareVector2 (const sf::Vector2<T> lhs, const sf::Vector2<T> rhs)
{
    if(lhs.x < rhs.x)
        return true;
    else if(lhs.x == rhs.x)
    {
        if(lhs.y < rhs.y)
            return true;
        else
            return false;
    }
    else
        return false;
}
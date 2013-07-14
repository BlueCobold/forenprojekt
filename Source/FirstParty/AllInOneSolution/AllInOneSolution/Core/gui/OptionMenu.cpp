#include "OptionMenu.hpp"
#include "../resources/Config.hpp"
#include <SFML/Audio/Listener.hpp>

OptionMenu::OptionMenu(const sf::Vector2f& position,
                       sf::RenderWindow& screen,
                       ResourceManager& resourceManager,
                       Config& config) :
    CaptionMenu(*resourceManager.getMenuTemplate("OptionMenu"), position, screen),
    m_config(config),
    m_fullScreen(false)
{
    m_fullScreen = m_config.get<bool>("IsFullScreen");

    m_masterVolume = m_config.get<float>("MasterVolume");
    sf::Listener::setGlobalVolume(m_masterVolume);

    Menu::getCheckboxes(CHECKBOX_FULLSCREEN).setChecked(m_fullScreen);

    Menu::getSlider(SLIDER_MASTERVOLUMEN).setValue(static_cast<float>(m_masterVolume));
}

void OptionMenu::applyChanges()
{

    if(Menu::getCheckboxes(CHECKBOX_FULLSCREEN).getChecked() != m_fullScreen)
    {
        sf::VideoMode videoMode(m_config.get<unsigned int>("ResolutionX"), m_config.get<unsigned int>("ResolutionY"));
        adjustVideoMode(videoMode, !m_fullScreen);

        if(!m_fullScreen)
            Menu::getRenderWindow().create(videoMode, utility::translateKey("gui_rickety_racquet"), sf::Style::Fullscreen);
        if(m_fullScreen)
            Menu::getRenderWindow().create(sf::VideoMode(videoMode), utility::translateKey("gui_rickety_racquet"));

        m_fullScreen = !m_fullScreen;
        m_config.set("IsFullScreen", m_fullScreen);
    }

    if(Menu::getSlider(SLIDER_MASTERVOLUMEN).getValue() != m_masterVolume)
    {
        m_masterVolume = Menu::getSlider(SLIDER_MASTERVOLUMEN).getValue();
        sf::Listener::setGlobalVolume(m_masterVolume);
        m_config.set("MasterVolume", m_masterVolume);
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

    m_masterVolume = m_config.get<float>("MasterVolume");
    sf::Listener::setGlobalVolume(m_masterVolume);

    Menu::getCheckboxes(CHECKBOX_FULLSCREEN).setChecked(m_fullScreen);

    Menu::getSlider(SLIDER_MASTERVOLUMEN).setValue(static_cast<float>(m_masterVolume));
}
#include "OptionMenu.hpp"
#include "../resources/Config.hpp"

OptionMenu::OptionMenu(const sf::Vector2f& position,
                       sf::RenderWindow& screen,
                       ResourceManager& resourceManager,
                       Config& config) :
    CaptionMenu(*resourceManager.getMenuTemplate("OptionMenu"), position, screen),
    m_config(config),
    m_fullScreen(false)
{
    m_fullScreen = m_config.get<bool>("IsFullScreen");

    for(auto it = begin(m_checkBoxes); it != end(m_checkBoxes); ++it)
        if(it->get()->getId() == CHECKBOX_FULLSCREEN)
            it->get()->setChecked(m_fullScreen);
}

void OptionMenu::applyChanges()
{
    for(auto it = begin(m_checkBoxes); it != end(m_checkBoxes); ++it)
    {
        
        if(it->get()->getId() == CHECKBOX_FULLSCREEN && (it->get()->getChecked() != m_fullScreen))
        {
            sf::VideoMode videoMode(m_config.get<unsigned int>("ResolutionX"), m_config.get<unsigned int>("ResolutionY"));
            adjustVideoMode(videoMode, it->get()->getChecked());

            if(it->get()->getChecked())
                m_screen.create(videoMode, utility::translateKey("gui_rickety_racquet"), sf::Style::Fullscreen);
            if(!it->get()->getChecked())
                m_screen.create(sf::VideoMode(videoMode), utility::translateKey("gui_rickety_racquet"));

            m_fullScreen = it->get()->getChecked();
        }
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
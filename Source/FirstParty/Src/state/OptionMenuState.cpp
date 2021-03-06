#include "OptionMenuState.hpp"

#include "../gui/Button.hpp"
#include "../gui/CustomContent.hpp"
#include "../model/Level.hpp"
#include "../resources/Config.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

OptionMenuState::OptionMenuState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 AppConfig& config) :
    m_screen(screen),
    State(screen, resourceManager, config),
    m_menu(screen, resourceManager, config),
    m_HUD(resourceManager, config),
    m_level(nullptr),
    m_clicked(-1),
    m_transitionStateInfo(OptionMenuStateId)
{
    auto buttonFunc = [&](const Button& sender){ m_clicked = sender.getId(); };
    m_menu.getButton(OptionMenu::BUTTON_ARROW_LEFT).registerOnPressed(buttonFunc);
    m_menu.getButton(OptionMenu::BUTTON_ARROW_RIGHT).registerOnPressed(buttonFunc);
    m_menu.getButton(OptionMenu::BUTTON_CLOSE).registerOnPressed(buttonFunc);
}

OptionMenuState::~OptionMenuState()
{
}

void OptionMenuState::onEnter(const EnterStateInformation* enterInformation, const double time)
{
    State::onEnter(enterInformation, time);

    const EnterOptionStateInformation* info = dynamic_cast<const EnterOptionStateInformation*>(enterInformation);
    m_transitionStateInfo.m_followingState = info->m_comeFromState;
    m_level = info->m_level;

    m_HUD.onEnter(m_level);

    m_menu.onEnter(m_screen);

    m_pauseStateInfo.m_levelNumber = enterInformation->m_levelNumber;

    m_menu.getLabel(OptionMenu::LABEL_RESOLUTION).setText(utility::translateKey("gui_label_resolution"));
    m_menu.update(m_screen, getPassedTime());
}

StateChangeInformation OptionMenuState::update(const double time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    m_clicked = -1;

    updateTime(time);
    m_menu.update(m_screen, getPassedTime());
    
    if(utility::Keyboard.isKeyReleased(sf::Keyboard::Escape))
        m_clicked = OptionMenu::BUTTON_CLOSE;

    if(m_clicked == OptionMenu::BUTTON_CLOSE)
    {
        m_pauseStateInfo.m_level = m_level;
        m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
        m_menu.applyChanges(m_screen);
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(m_clicked == OptionMenu::BUTTON_ARROW_LEFT)
        m_menu.prevVideoMode();
    else if(m_clicked == OptionMenu::BUTTON_ARROW_RIGHT)
        m_menu.nextVideoMode();
    
    return StateChangeInformation::Empty();
}

void OptionMenuState::doDraw(const DrawParameter& params)
{
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.getCustomContent(-2).registerOnDraw([&](int id, const DrawParameter& p)
    {
        if(m_level != nullptr)
        {
            m_level->adjustView(p);
            m_HUD.update(m_level, getPassedTime());
            m_level->draw(p);
            m_HUD.draw(p);
            whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
            p.getTarget().draw(whiteRect);
        }
    });
    m_menu.draw(params);
}

#include "OptionMenuState.hpp"

#include "../gui/Button.hpp"
#include "../model/Level.hpp"
#include "../resources/Config.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

OptionMenuState::OptionMenuState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager, config),
    m_HUD(resourceManager, config),
    m_level(nullptr),
    m_clicked(-1)
{
    auto buttonFunc = [&](const Button& sender){ m_clicked = sender.getId(); };
    m_menu.getButton(OptionMenu::BUTTON_ARROW_LEFT).registerOnPressed(buttonFunc);
    m_menu.getButton(OptionMenu::BUTTON_ARROW_RIGHT).registerOnPressed(buttonFunc);
    m_menu.getButton(OptionMenu::BUTTON_CLOSE).registerOnPressed(buttonFunc);
}

OptionMenuState::~OptionMenuState()
{
}

void OptionMenuState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation,time);

    const EnterOptionStateInformation* info = dynamic_cast<const EnterOptionStateInformation*>(enterInformation);
    m_transitionStateInfo.m_followingState = info->m_comeFromState;
    m_level = info->m_level;

    m_menu.onEnter();
    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    m_pauseStateInfo.m_levelNumber = enterInformation->m_levelNumber;

    m_menu.getLabel(OptionMenu::LABEL_RESOLUTION).setText(utility::translateKey("gui_label_resolution"));
}

StateChangeInformation OptionMenuState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    m_clicked = -1;

    m_menu.update(m_screen);

    if(m_clicked == OptionMenu::BUTTON_CLOSE)
    {
        m_pauseStateInfo.m_level = m_level;
        m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
        m_menu.applyChanges();
        m_transitionStateInfo.m_comeFromeState = OptionMenuStateId;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(m_clicked == OptionMenu::BUTTON_ARROW_LEFT)
        m_menu.prevVideoMode();
    else if(m_clicked == OptionMenu::BUTTON_ARROW_RIGHT)
        m_menu.nextVideoMode();

    return StateChangeInformation::Empty();
}

void OptionMenuState::draw(const DrawParameter& params)
{
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    if(m_level != nullptr)
    {
        m_level->adjustView(params.getTarget());
        m_HUD.update(m_level, getCurrentTime());
        m_level->draw(params);
        m_HUD.draw(params);
        whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    }
    else
        whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}
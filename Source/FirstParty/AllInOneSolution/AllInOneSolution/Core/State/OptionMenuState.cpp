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
    m_level(nullptr)
{
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
}

StateChangeInformation OptionMenuState::update(const float time)
{ 
    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen);

    if(clicked == OptionMenu::BUTTON_CLOSE)
    {
        m_pauseStateInfo.m_level = m_level;
        m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
        m_menu.applyChanges();
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

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
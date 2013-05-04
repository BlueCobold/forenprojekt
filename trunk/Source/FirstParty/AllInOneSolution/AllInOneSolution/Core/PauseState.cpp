#include "PauseState.hpp"
#include "Input.hpp"
#include "resources/Config.hpp"
#include "resources/ResourceManager.hpp"
#include "rendering\transitions\RandomTransition.hpp"

#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

PauseState::PauseState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config, utility::Event& incident) :
    State(screen, resourceManager, config, incident), m_background(nullptr),
    m_gameMenu(sf::Vector2f(0, 0), "Game Paused", GameMenu::Continue, screen, resourceManager),
    m_HUD(resourceManager, config)
{
}

PauseState::~PauseState()
{
}

void PauseState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    const EnterPauseStateInformation* info = dynamic_cast<const EnterPauseStateInformation*>(enterInformation);
    m_level = info->m_level;
    
    m_timeDiff = time - info->m_levelTime;
    State::onEnter(enterInformation, time - m_timeDiff);
    m_HUD.restartAt(getCurrentTime());

    m_gameMenu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_gameMenu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_gameMenu.getSize().y / 2.f));
}

StateChangeInformation PauseState::update(const float time)
{
    if(m_event.m_eventType == utility::Event::Resized)
    {   
        m_gameMenu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_gameMenu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_gameMenu.getSize().y / 2.f));
        m_event.m_eventType = utility::Event::NoEvent;
    }

    updateTime(time - m_timeDiff);

    m_gameMenu.update(m_screen);

    if(utility::Keyboard.isKeyDown(sf::Keyboard::P) || utility::Keyboard.isKeyDown(sf::Keyboard::Pause)
        || utility::Keyboard.isKeyDown(sf::Keyboard::Escape) || m_gameMenu.isPressed(GameMenu::Continue))
    {
        m_playStateInfo.m_returnFromPause = true;
        m_playStateInfo.m_level = m_level;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = PlayStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    
    return StateChangeInformation::Empty();
}

void PauseState::draw(const DrawParameter& params)
{
    m_level->adjustView(params.getTarget());
    m_level->draw(params);
    m_HUD.update(m_level, getCurrentTime());
    m_HUD.draw(params);
    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);

    m_gameMenu.draw(params);
}

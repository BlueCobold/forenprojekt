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

PauseState::PauseState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
    State(screen, resourceManager, config), m_background(nullptr),
    m_label(sf::Vector2f(0, 0), 0, resourceManager.getBitmapFont("green"), HUDElement::Center, HUDElement::Center, "game paused", LineLabel::Centered),
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
}

StateChangeInformation PauseState::update(const float time)
{
    updateTime(time - m_timeDiff);

    if(utility::Keyboard.isKeyDown(sf::Keyboard::P) || utility::Keyboard.isKeyDown(sf::Keyboard::Pause)
        || utility::Keyboard.isKeyDown(sf::Keyboard::Escape))
    {
        m_playStateInfo.m_returnFromPause = true;
        m_playStateInfo.m_level = m_level;
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

    m_label.update(params.getTarget());
    m_label.draw(params);
}

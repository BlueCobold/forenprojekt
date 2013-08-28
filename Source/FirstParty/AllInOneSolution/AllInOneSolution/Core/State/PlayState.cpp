#include "PlayState.hpp"
#include "../Input.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

PlayState::PlayState(sf::RenderWindow& screen, 
                     ResourceManager& resourceManager, 
                     AppConfig& config) :
    State(screen, resourceManager, config),
    m_level(nullptr),
    m_hud(resourceManager, config),
    m_shouldPause(false)
{
}

PlayState::~PlayState()
{

}

void PlayState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    m_shouldPause = false;
    State::onEnter(enterInformation, time);

    const EnterPlayStateInformation* info = dynamic_cast<const EnterPlayStateInformation*>(enterInformation);
    m_level = info->m_level;
    
    utility::Mouse.capture();
    m_level->adaptToMouse();

    if(!info->m_returnFromPause)
    {
        m_level->restartAt(getCurrentTime());
        m_level->update(getCurrentTime(), m_screen);
        m_level->onEnter();
        m_hud.update(m_level, getCurrentTime());
        m_timeShift = 0.0f;
        m_hud.skipInterpolation();
    }

    m_pauseStateInfo.m_levelNumber = enterInformation->m_levelNumber;
}

StateChangeInformation PlayState::update(const float time)
{
    updateTime(time);

    if(!isPaused())
    {
        m_level->update(getCurrentTime(), m_screen);
        m_hud.update(m_level, getCurrentTime());
        //if(utility::Keyboard.isKeyDown(sf::Keyboard::R))
        //{
        //    // BUG! Memory leak!
        //    m_level = new Level(2, m_resourceManager, m_config);
        //    m_level->restartAt(getCurrentTime());
        //}

        if(utility::Keyboard.isKeyDown(sf::Keyboard::T))
        {
            m_level->setTimeAttackMode(true);
        }

        if(m_shouldPause || utility::Keyboard.isKeyDown(sf::Keyboard::P) || utility::Keyboard.isKeyDown(sf::Keyboard::Pause))
        {
            m_pauseStateInfo.m_levelTime = getCurrentTime();
            m_pauseStateInfo.m_level = m_level;
            m_transitionStateInfo.m_level = m_level;
            m_transitionStateInfo.m_followingState = PauseStateId;
            m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
        }

        if(m_level->isLevelPassed())
        {
            m_pauseStateInfo.m_levelTime = getCurrentTime();
            m_pauseStateInfo.m_level = m_level;
            m_transitionStateInfo.m_followingState = LevelPassStateId;
            m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
        }

        if(m_level->isLevelFailed())
        {
            m_pauseStateInfo.m_levelTime = getCurrentTime();
            m_pauseStateInfo.m_level = m_level;
            m_transitionStateInfo.m_followingState = LevelFailStateId;
            m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
        }
    }

    return StateChangeInformation::Empty();
}

void PlayState::draw(const DrawParameter& params)
{
    m_level->adjustView(params.getTarget());
    m_level->draw(params);
    m_hud.draw(params);
}

void PlayState::onEvent(utility::Event::EventType type)
{
    if(type == utility::Event::LostFocus)
        m_shouldPause = true;
}

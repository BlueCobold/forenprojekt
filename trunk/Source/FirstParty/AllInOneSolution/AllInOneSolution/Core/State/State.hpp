#pragma once

#ifndef STATE_HPP
#define STATE_HPP

class AppConfig;
class ResourceManager;

namespace sf
{
    class RenderWindow;
}

enum StateId
{
    None,
    PlayStateId,
    LoadLevelStateId,
    PauseStateId,
    TransitionStateId,
    LevelPassStateId,
    LevelFailStateId,
    MainMenuStateId,
    StartStateId,
    OptionMenuStateId,
    CreditMenuStateId,
    LevelPreviewStateId
};

#include "StateChangeInformation.hpp"
#include "../Input.hpp"
#include "../Utility.hpp"
#include "../rendering/Drawable.hpp"

/// This class will be used to discribe a State
/// and to be managed by the StateManager
class State : public Drawable
{
public:
    
    State(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config) :
        m_screen(screen),
        m_resourceManager(resourceManager),
        m_pause(false),
        m_pauseDelay(0),
        m_config(config)
    { }

    virtual ~State()
    { }

    virtual void draw(const DrawParameter& params) = 0;
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time)
    {
        utility::Mouse.capture();
        updateTime(time);
        m_enterTime = time;
    }

    virtual StateChangeInformation update(const float time) = 0;

    void pause(const float time)
    {
        if(!isPaused())
            m_pauseStart = time;
        m_pause = true;
    }

    void resume(const float time)
    {
        if(isPaused())
            m_pauseDelay += time - m_pauseStart;
        m_pause = false;
    }

    const bool isPaused()
    {
        return m_pause;
    }

    virtual void onEvent(utility::Event::EventType type)
    {
    }

protected:
    
    void updateTime(const float time)
    {
        if(isPaused())
            return;
        m_currentTime = time;
    }

    float getCurrentTime() const
    {
        return m_currentTime - m_pauseDelay;
    }

    float getPassedTime() const
    {
        return m_currentTime - m_enterTime;
    }

    sf::RenderWindow& m_screen;
    ResourceManager& m_resourceManager;
    AppConfig& m_config;

private:
    bool m_pause;
    float m_enterTime;
    float m_pauseStart;
    float m_currentTime;
    float m_pauseDelay;
};

#endif // STATE_HPP

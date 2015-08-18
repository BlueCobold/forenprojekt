#pragma once

#ifndef STATE_HPP
#define STATE_HPP

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
    LevelPreviewStateId,
    LevelSelectStateId,
    CoinShopStateId,
    HighScoreStateId,
    NewHighScoreStateId,
    GameFinishedStateId,
    AchievementStateId
};

#include "StateChangeInformation.hpp"
#include "../Input.hpp"
#include "../Utility.hpp"
#include "../rendering/Drawable.hpp"
#include "../resources/AppConfig.hpp"
#include <SFML/Audio/Listener.hpp>

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
        m_config(config),
        m_currentTime(0)
    {
        pause(m_currentTime);
    }

    virtual ~State()
    { }

    virtual void draw(const DrawParameter& params) = 0;
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time)
    {
        utility::Mouse.capture();
        utility::Mouse.showSystemCursor(m_screen);
        updateTime(time);
    }

    virtual StateChangeInformation update(const float time) = 0;

    void pause(const float time)
    {
        if(!isPaused())
            m_pauseStart = time;
        m_pause = true;
#ifndef NO_SOUND
        if(m_config.get<bool>("MuteSoundWhenInactiv"))
           sf::Listener::setGlobalVolume(0.0f);
#endif
    }

    void resume(const float time)
    {
        if(isPaused())
            m_pauseDelay += time - m_pauseStart;
        m_pause = false;
#ifndef NO_SOUND
        sf::Listener::setGlobalVolume(m_config.get<float>("MasterVolume"));
#endif
    }

    const bool isPaused()
    {
        return m_pause;
    }

    virtual void onEvent(utility::Event::EventType type, const float time)
    {
        if(type == utility::Event::LostFocus)
            pause(time);
        else if(type == utility::Event::GainFocus)
            resume(time);
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
        return m_currentTime;
    }

    float getPassedTime() const
    {
        return m_currentTime - m_pauseDelay;
    }

    ResourceManager& getResourceManager() const
    {
        return m_resourceManager;
    }

    sf::RenderWindow& m_screen;
    AppConfig& m_config;

private:
    ResourceManager& m_resourceManager;
    bool m_pause;
    float m_pauseStart;
    float m_currentTime;
    float m_pauseDelay;
};

#endif // STATE_HPP
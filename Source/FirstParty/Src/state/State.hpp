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
        m_config(config),
        m_resourceManager(resourceManager),
        m_pause(false),
        m_currentTime(0),
        m_pauseDelay(0)
    {
        pause(m_currentTime);
    }

    virtual ~State()
    { }

    virtual void onEnter(const EnterStateInformation* enterInformation, const double time)
    {
        utility::Mouse.capture();
        utility::Mouse.showSystemCursor(m_screen);
        updateTime(time);
    }

    virtual StateChangeInformation update(const double time) = 0;

    void pause(const double time)
    {
        if(!isPaused())
            m_pauseStart = time;
        m_pause = true;
#ifndef NO_SOUND
        if(m_config.get<bool>("MuteSoundWhenInactiv"))
           sf::Listener::setGlobalVolume(0.0f);
#endif
    }

    void resume(const double time)
    {
        if(isPaused())
            m_pauseDelay += time - m_pauseStart;
        m_pause = false;
#ifndef NO_SOUND
        sf::Listener::setGlobalVolume(100.f);
#endif
    }

    const bool isPaused()
    {
        return m_pause;
    }

    virtual void onEvent(utility::Event::EventType type, const double time)
    {
        if(type == utility::Event::LostFocus)
            pause(time);
        else if(type == utility::Event::GainFocus)
            resume(time);
    }

protected:
    void updateTime(const double time)
    {
        if(isPaused())
            return;
        m_currentTime = time;
    }

    double getCurrentTime() const
    {
        return m_currentTime;
    }

    double getPassedTime() const
    {
        return m_currentTime - m_pauseDelay;
    }

    ResourceManager& getResourceManager()
    {
        return m_resourceManager;
    }

    AppConfig& getConfig()
    {
        return m_config;
    }

    sf::RenderWindow& m_screen;
    AppConfig& m_config;

private:
    ResourceManager& m_resourceManager;
    bool m_pause;
    double m_pauseStart;
    double m_currentTime;
    double m_pauseDelay;
};

#endif // STATE_HPP

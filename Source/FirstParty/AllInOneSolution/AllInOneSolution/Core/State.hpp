#pragma once

#ifndef STATE_HPP
#define STATE_HPP

class Config;
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
};

#include "StateChangeInformation.hpp"

/// This class will be used to discribe a State
/// and to be managed by the StateManager
class State
{
public:
    
    State(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
        m_screen(screen),
        m_resourceManager(resourceManager),
        m_pause(false),
        m_config(config)
    { }

    virtual ~State()
    { }

    virtual StateChangeInformation update() = 0;
    virtual void draw() = 0;
	virtual void onEnter(void *enterInformation) = 0;

    void pause()
    {
         m_pause = true;
    }

    void resume()
    {
         m_pause = false;
    }

    const bool isPaused()
    {
        return m_pause;
    }

protected:
    
    sf::RenderWindow& m_screen;
    ResourceManager& m_resourceManager;
    Config& m_config;

private:

    bool m_pause;

};

#endif // STATE_HPP

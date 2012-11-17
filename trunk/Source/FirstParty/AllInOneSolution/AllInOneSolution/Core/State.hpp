#pragma once

#ifndef STATE_HPP
#define STATE_HPP

namespace sf
{
    class RenderWindow;
}

/// This class will be used to discribe a State
/// and to be managed by the StateManager
class State
{
public:
    
    State(sf::RenderWindow& screen);

    virtual ~State();

    virtual bool initialize() = 0;

    virtual void update() = 0;

    virtual void draw() = 0;

    virtual void shutdown() = 0;

    void pause();

    void resume();

    const bool isPaused();

private:

    bool m_pause;

protected:
    
    sf::RenderWindow& m_screen;

};

#endif // STATE_HPP

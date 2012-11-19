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
    
    State(sf::RenderWindow& screen) :
        m_screen(screen),
        m_pause(false)
    { }

    virtual ~State()
    { }

    virtual void update() = 0;
    virtual void draw() = 0;

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

private:

    bool m_pause;

};

#endif // STATE_HPP

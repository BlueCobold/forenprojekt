#pragma once

#include <SFML/Graphics.hpp>

/// This class will be used to discribe a State
/// and to be managed by the StateManager
class State
{
    public:
    
    State(sf::RenderWindow& screen);
    ~State();

    virtual bool initialize() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void shutdown() = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;

    private:

    sf::RenderWindow& m_screen;

};


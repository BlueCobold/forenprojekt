#pragma once
#include "State.hpp"

/// This class will be used to test the State Managing System
class TestState : public State
{
public:

    TestState(sf::RenderWindow& screen);
    ~TestState();

    bool initialize();
    void update();
    void draw();
    void shutdown();

    private:

    sf::CircleShape m_circle;
       
};


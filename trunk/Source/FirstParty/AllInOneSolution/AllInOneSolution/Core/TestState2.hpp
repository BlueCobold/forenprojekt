#pragma once
#include "State.hpp"

/// This class will be used to test the State Managing System
class TestState2 : public State
{
public:

    TestState2(sf::RenderWindow& screen);
    ~TestState2();

    bool initialize();
    void update();
    void draw();
    void shutdown();

    private:

    sf::CircleShape m_circle;
       
};


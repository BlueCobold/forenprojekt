#pragma once

#ifndef TESTSTATE_HPP
#define TESTSTATE_HPP

#include "State.hpp"
#include "AnimatedEntity.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

namespace sf
{
    class RenderWindow;
}

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

    sf::Clock m_clock;
    sf::Texture m_texture;
    AnimatedEntity m_animation;
};

#endif // TESTSTATE_HPP

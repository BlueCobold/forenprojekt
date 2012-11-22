#pragma once

#ifndef TESTSTATE_HPP
#define TESTSTATE_HPP

#include "State.hpp"
#include "Entity.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

class StateManager;
class ResourceManager;

namespace sf
{
    class RenderWindow;
}

class TestState : public State
{
public:

    TestState(StateManager& stateManager, sf::RenderWindow& screen, ResourceManager& resourceManager);
    ~TestState();

    void update();
    void draw();

private:

    sf::Clock m_clock;
    sf::Texture m_texture;
    Entity m_animation;

    StateManager& m_stateManager;
};

#endif // TESTSTATE_HPP

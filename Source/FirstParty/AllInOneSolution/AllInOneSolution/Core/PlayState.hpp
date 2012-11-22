#pragma once

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "State.hpp"
#include "Level.hpp"

#include <memory> // unique_ptr

class ResourceManager;

/// The PlayState is where the actual gameplay happens.
class PlayState : public State
{
public:
    PlayState(sf::RenderWindow& screen, ResourceManager& resourceManager);
    ~PlayState();

    void update();
    void draw();

private:
    std::unique_ptr<Level> m_level;
};

#endif // PLAYSTATE_HPP
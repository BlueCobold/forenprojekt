#pragma once

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "State.hpp"

/// The PlayState is where the actual gameplay happens.
class PlayState : public State
{
public:
    PlayState(sf::RenderWindow& screen);
    ~PlayState();

    void update();
    void draw();

private:

};

#endif // PLAYSTATE_HPP
#pragma once

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "State.hpp"
#include "model/Level.hpp"
#include "gui/HUD.hpp"

#include <SFML/System/Clock.hpp>

#include <memory> // unique_ptr

class ResourceManager;

/// The PlayState is where the actual gameplay happens.
class PlayState : public State
{
public:

    PlayState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config);
    ~PlayState();

    StateChangeInformation update();
    void draw();
	void onEnter(void *enterInformation);

private:

    std::unique_ptr<Level> m_level;
    sf::Clock m_frametime;

    HUD m_hud;

};

#endif // PLAYSTATE_HPP
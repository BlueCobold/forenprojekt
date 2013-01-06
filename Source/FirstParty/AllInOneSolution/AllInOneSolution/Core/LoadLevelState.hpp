#pragma once

#ifndef LOADLEVELSTATE_HPP
#define LOADLEVELSTATE_HPP

#include "State.hpp"
#include "model/Level.hpp"
#include "model\AlphaFadedGraphics.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory> // unique_ptr

class ResourceManager;

/// The State where the level will be loaded and faded in.
class LoadLevelState : public State
{
public:

    LoadLevelState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config);
    ~LoadLevelState();

    StateChangeInformation update();
    void draw();
	void onEnter(void *enterInformation);

private:

    Level *m_level;
	std::unique_ptr<AlphaFadedGraphics> m_alphaFadedScreen;
	sf::Clock m_frametime;
};

#endif // LOADLEVELSTATE_HPP
#pragma once

#ifndef LOADLEVELSTATE_HPP
#define LOADLEVELSTATE_HPP

#include "State.hpp"
#include "model/Level.hpp"
#include "rendering/transitions/Transition.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

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
    std::unique_ptr<Transition> m_fadedScreen;
    sf::Clock m_frametime;
    sf::RenderTexture m_texture;

    /// for testing
    Level *m_level2; 
    sf::RenderTexture m_texture2;
};

#endif // LOADLEVELSTATE_HPP
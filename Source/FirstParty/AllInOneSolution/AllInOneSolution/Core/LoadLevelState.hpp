#pragma once

#ifndef LOADLEVELSTATE_HPP
#define LOADLEVELSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "model/Level.hpp"
#include "gui/hud/HUD.hpp"
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

    virtual void draw(const DrawParameter& params);

    StateChangeInformation update(const float time);
    void onEnter(const EnterStateInformation *enterInformation, const float time);
    std::unique_ptr<Level> gainLevel();

private:

    std::unique_ptr<Level> m_level;
    Level* m_lastLevel;

    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
};

#endif // LOADLEVELSTATE_HPP
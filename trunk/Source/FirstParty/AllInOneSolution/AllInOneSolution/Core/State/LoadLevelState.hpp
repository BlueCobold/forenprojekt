#pragma once

#ifndef LOADLEVELSTATE_HPP
#define LOADLEVELSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../model/Level.hpp"
#include "../gui/hud/HUD.hpp"
#include "../rendering/transitions/Transition.hpp"
#include "../resources/ResourceManager.hpp"
#include "../Utility.hpp"
#include "../resources/BackgroundLoader.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Thread.hpp>

#include <memory> // unique_ptr
#include <string>

/// The State where the level will be loaded and faded in.
class LoadLevelState : public State
{
public:

    LoadLevelState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~LoadLevelState();

    virtual void draw(const DrawParameter& params);

    StateChangeInformation update(const float time);
    void onEnter(const EnterStateInformation *enterInformation, const float time);
    std::unique_ptr<Level> gainLevel();

private:

    std::unique_ptr<Level> m_level;
    Level* m_lastLevel;
    std::string m_loadingErrorMessage;

    LineLabel m_label;
    int m_currentLevel;
    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;

    std::unique_ptr<BackgroundLoader<LoadLevelState>> m_loadingLevel;
    void loadLevel();
    void onEvent(utility::Event::EventType type);
};

#endif // LOADLEVELSTATE_HPP

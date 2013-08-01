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

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Thread.hpp>

#include <memory> // unique_ptr

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
    char m_loadingErrorMessage[200];

    LineLabel m_label;
    int m_currentLevel;
    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;

    bool m_loaded;
    bool m_loadInProgress;

    std::unique_ptr<sf::Thread> loadingLevelThread;

    void loadLevel();
    void onEvent(utility::Event::EventType type);
};

#endif // LOADLEVELSTATE_HPP

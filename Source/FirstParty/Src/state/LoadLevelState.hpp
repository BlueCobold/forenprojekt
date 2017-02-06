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

#include <functional>
#include <memory>
#include <string>

/// The State where the level will be loaded and faded in.
class LoadLevelState : public State
{
public:
    typedef std::function<void(std::unique_ptr<Level>&)> LoadCallback; // & should not exist, VS2010 bug requires it
    LoadLevelState(sf::RenderWindow& screen,
                   ResourceManager& resourceManager,
                   AppConfig& config,
                   LoadCallback onLevelLoaded);
    ~LoadLevelState();

    StateChangeInformation update(const double time) override;
    void onEnter(const EnterStateInformation *enterInformation, const double time) override;

private:
    void loadLevel();
    void onEvent(utility::Event::EventType type);
    void doDraw(const DrawParameter& params) override;

    LineLabel m_label;
    Level* m_lastLevel;
    std::string m_loadingErrorMessage;
    std::string m_file;
    int m_currentLevel;
    bool m_directPlay;
    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    LoadCallback m_onLevelLoaded;

    std::unique_ptr<BackgroundLoader<LoadLevelState>> m_levelLoaderJob;
};

#endif // LOADLEVELSTATE_HPP

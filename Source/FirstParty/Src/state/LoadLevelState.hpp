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

    StateChangeInformation update(const float time) override;
    void onEnter(const EnterStateInformation *enterInformation, const float time) override;
    std::unique_ptr<Level> gainLevel();

    void setLanguage(const std::string& language) override;

private:
    void loadLevel();
    void onEvent(utility::Event::EventType type);
    void doDraw(const DrawParameter& params) override;

    LineLabel m_label;
    std::unique_ptr<Level> m_level;
    Level* m_lastLevel;
    std::string m_loadingErrorMessage;
#ifdef LEVELTESTING
    std::string m_file;
#endif
    int m_currentLevel;
    bool m_directPlay;
    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;

    std::unique_ptr<BackgroundLoader<LoadLevelState>> m_levelLoaderJob;
};

#endif // LOADLEVELSTATE_HPP

#pragma once

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/hud/HUD.hpp"
#include "../resources/BackgroundLoader.hpp"
class Level;

#include <SFML/System/Thread.hpp>
#include <SFML/System/Clock.hpp>

#include <memory> // unique_ptr

class ResourceManager;

/// The PlayState is where the actual gameplay happens.
class PlayState : public State
{
public:

    PlayState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~PlayState();

    StateChangeInformation update(const float time) override;
    void onEnter(const EnterStateInformation* enterInformation, const float time) override;
    virtual void onEvent(utility::Event::EventType type);

private:
    void doDraw(const DrawParameter& params) override;

    Level* m_level;
    HUD m_hud;
    bool m_shouldPause;

    int m_minPoints;

    std::unique_ptr<BackgroundLoader<PlayState>> m_onlineHighScoreLoaderJob;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterPauseStateInformation m_pauseStateInfo;
    EnterLoadLevelStateInformation m_loadLevelStateInfo;

    bool checkForNewHighscore();

    void loadOnlineHighscore();
};

#endif // PLAYSTATE_HPP

#pragma once

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../model/Level.hpp"
#include "../gui/hud/HUD.hpp"
#include <SFML/System/Thread.hpp>
#include <SFML/System/Clock.hpp>
#include "../resources/BackgroundLoader.hpp"
#include <memory> // unique_ptr

class ResourceManager;

/// The PlayState is where the actual gameplay happens.
class PlayState : public State
{
public:

    PlayState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~PlayState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);
    virtual void onEvent(utility::Event::EventType type);

private:

    Level* m_level;
    HUD m_hud;
    bool m_shouldPause;

    int m_minPoints;

    std::unique_ptr<BackgroundLoader<PlayState>> m_loadingOnlineHighScore;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterPauseStateInformation m_pauseStateInfo;
    EnterStateInformation m_loadLevelStateInfo;

    bool checkForNewHighscore();

    void loadOnlineHighscore();
};

#endif // PLAYSTATE_HPP

#pragma once

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../model/Level.hpp"
#include "../gui/hud/HUD.hpp"

#include <SFML/System/Clock.hpp>

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

    EnterTransitionStateInformation m_transitionStateInfo;
    EnterPauseStateInformation m_pauseStateInfo;
    float m_timeShift;

    bool PlayState::checkForNewHighscore();
};

#endif // PLAYSTATE_HPP

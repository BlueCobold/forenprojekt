#pragma once

#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

/// This is the active game state when the game is paused.
class PauseState : public State
{
public:

    PauseState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config);
    ~PauseState();

    StateChangeInformation update();
    void draw();
    void onEnter(void *enterInformation);

private:

    std::unique_ptr<sf::Texture> m_background;
    const sf::Texture* m_foreground;
    sf::RenderTexture m_renderTexture;
    Level* m_level;
    LineLabel m_label;

    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterPauseStateInformation m_pauseStateInfo;
    bool m_enterPauseTransition;
};

#endif // PAUSESTATE_HPP
#pragma once

#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "gui/LabelHUD.hpp"

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

    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);
private:

    std::unique_ptr<sf::Texture> m_background;
    sf::RenderTexture m_renderTexture;
    std::unique_ptr<Level> m_level;
    LabelHUD m_label;

    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterPauseStateInformation m_pauseStateInfo;
    bool m_enterPauseTransition;
};

#endif // PAUSESTATE_HPP
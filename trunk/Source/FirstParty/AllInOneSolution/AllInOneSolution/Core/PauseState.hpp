#pragma once

#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "gui/hud/HUD.hpp"
#include "gui/GameMenu.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

/// This is the active game state when the game is paused.
class PauseState : public State
{
public:

    PauseState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config);
    ~PauseState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
	virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);

    std::unique_ptr<sf::Texture> m_background;
    sf::RenderTexture m_renderTexture;
    Level* m_level;

    HUD m_HUD;
    GameMenu m_gameMenu;

    sf::Vector2u m_screenSize;

    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterPauseStateInformation m_pauseStateInfo;
    float m_timeDiff;
};

#endif // PAUSESTATE_HPP
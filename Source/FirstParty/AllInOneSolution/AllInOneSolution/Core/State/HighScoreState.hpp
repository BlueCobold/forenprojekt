#pragma once

#ifndef HIGHSCORESTATE_HPP
#define HIGHSCORESTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/Button.hpp"
#include "../gui/HighScoreMenu.hpp"
#include "../gui/hud/HUD.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

class HighScoreState : public State
{
public:

    HighScoreState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~HighScoreState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);
    void loadHighScore(Level& level);
    HighScoreMenu m_menu;
    HUD m_HUD;

    EnterPauseStateInformation m_stateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterHighScoreStateInformation m_highScoreStateInfo;
    float m_timeDiff;
};

#endif // HIGHSCORESTATE_HPP
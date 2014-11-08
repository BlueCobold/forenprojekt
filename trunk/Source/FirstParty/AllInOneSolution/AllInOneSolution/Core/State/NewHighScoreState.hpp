#pragma once

#ifndef NEWHIGHSCORESTATE_HPP
#define NEWHIGHSCORESTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/Button.hpp"
#include "../gui/hud/HUD.hpp"
#include "../gui/ReplayMenu.hpp"
#include "../gui/NewHighScoreMenu.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

/// This is the active game state when the level is done.
class NewHighScoreState : public State
{
public:

    NewHighScoreState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~NewHighScoreState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);
    void addNewHighScore(int points, std::string name);

    void sendDataToHighScoreServer(const std::string& name);

    std::unique_ptr<sf::Texture> m_background;
    sf::RenderTexture m_renderTexture;
    Level* m_level;

    HUD m_HUD;
    bool m_replay;

    NewHighScoreMenu m_menu;

    EnterPauseStateInformation m_stateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
};

#endif // NEWHIGHSCORESTATE_HPP

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

    StateChangeInformation update(const double time) override;
    void onEnter(const EnterStateInformation* enterInformation, const double time) override;

private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);
    void addNewHighScore(int points, double time, std::string name);
    void doDraw(const DrawParameter& params) override;

    void sendDataToHighScoreServer(const std::string& name) const;

    const std::string createHighscoreString(const std::string& name) const;

    std::unique_ptr<sf::Texture> m_background;
    NewHighScoreMenu m_menu;
    HUD m_HUD;

    sf::RenderTexture m_renderTexture;

    Level* m_level;
    bool m_replay;
    std::string m_lastName;

    EnterPauseStateInformation m_stateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
};

#endif // NEWHIGHSCORESTATE_HPP

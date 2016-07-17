#pragma once

#ifndef HIGHSCORESTATE_HPP
#define HIGHSCORESTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/Button.hpp"
#include "../gui/HighScoreMenu.hpp"
#include "../gui/hud/HUD.hpp"
#include "../resources/BackgroundLoader.hpp"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Thread.hpp>

#include <memory>

class HighScoreState : public State
{
public:

    HighScoreState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~HighScoreState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

    void setLanguage(const std::string& language) override;
private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);
    void loadHighScore();
    void clearHighScore();
    void loadOnlineHighscore();
    void buildSubWindowElements();

    HighScoreMenu m_menu;
    HUD m_HUD;

    bool m_showPoints;
    bool m_offsetAdded;
    sf::Vector2f m_offset;
    int m_clicked;

    EnterPauseStateInformation m_stateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterHighScoreStateInformation m_highScoreStateInfo;
    float m_timeDiff;

    bool m_onlineHighscore;

    std::unique_ptr<BackgroundLoader<HighScoreState>> m_onlineHighscoreLoaderJob;
};

#endif // HIGHSCORESTATE_HPP

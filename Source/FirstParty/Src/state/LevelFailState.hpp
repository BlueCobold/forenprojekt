#pragma once

#ifndef LEVELFAILSTATE_HPP
#define LEVELFAILSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/Button.hpp"
#include "../gui/FailMenu.hpp"
#include "../gui/hud/HUD.hpp"
class Level;

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

class AchievementManager;

/// This is the active game state when the level is done.
class LevelFailState : public State
{
public:

    LevelFailState(sf::RenderWindow& screen,
        ResourceManager& resourceManager,
        AppConfig& config,
        AchievementManager& achievementManager);
    ~LevelFailState();

    StateChangeInformation update(const double time) override;
    void onEnter(const EnterStateInformation* enterInformation, const double time) override;

private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);
    void setAchievements();
    void doDraw(const DrawParameter& params) override;

    AchievementManager& m_achievementManager;

    std::unique_ptr<sf::Texture> m_background;
    sf::RenderTexture m_renderTexture;
    Level* m_level;

    FailMenu m_menu;
    HUD m_HUD;
    bool m_replay;

    EnterPlayStateInformation m_playStateInfo;
    EnterHighScoreStateInformation m_highScoreInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterLoadLevelStateInformation m_loadLevelStateInfo;
};

#endif // LEVELFAILSTATE_HPP

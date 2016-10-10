#pragma once

#ifndef LEVELPASSSTATE_HPP
#define LEVELPASSSTATE_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"
#include "../gui/hud/HUD.hpp"
#include "../gui/ReplayMenu.hpp"

#include <SFML/Graphics/RenderTexture.hpp>

#include <memory>

class AchievementManager;

/// This is the active game state when the level is done.
class LevelPassState : public State
{
public:

    LevelPassState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config, AchievementManager& achievementManager);
    ~LevelPassState();

    StateChangeInformation update(const float time) override;
    void onEnter(const EnterStateInformation* enterInformation, const float time) override;

    void setLanguage(const std::string& language) override;

private:
    void renderStateTexture();
    void render(sf::RenderTarget& m_renderTexture);
    void addNewHighScore(int points, std::string name);
    void setAchievements();
    void doDraw(const DrawParameter& params) override;

    AchievementManager& m_achievementManager;

    std::unique_ptr<sf::Texture> m_background;
    sf::RenderTexture m_renderTexture;
    Level* m_level;

    ReplayMenu m_menu;
    HUD m_HUD;
    bool m_replay;
    bool m_gotCoins;

    EnterStateInformation m_stateInfo;
    EnterPlayStateInformation m_playStateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;
    EnterHighScoreStateInformation m_highScoreInfo;
    EnterLoadLevelStateInformation m_loadLevelStateInfo;

    void setMedal(int medal);
};

#endif // LEVELPASSSTATE_HPP

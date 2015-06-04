#pragma once

#ifndef ACHIEVEMENT_STATE_HPP
#define ACHIEVEMENT_STATE_HPP

#include "State.hpp"
#include "../gui/AchievementMenu.hpp"
#include "../resources/Achievement.hpp"
#include "../gui/hud/HUD.hpp"

class Level;
class AchievementManager;

class AchievementState : public State
{
public:

    AchievementState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config, AchievementManager& achievementManager);
    ~AchievementState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    AchievementMenu m_menu;
    Level* m_level;
    bool m_levelUpdated;
    HUD m_HUD;

    AchievementManager& m_achievementManager;
    std::unordered_map<std::string, Achievement>::const_iterator m_currentAchievementIterator;

    EnterStateInformation m_stateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;

    void updateLeftButton();
    void updateRightButton();

    void updateButtons();
};

#endif // ACHIEVEMENT_STATE_HPP
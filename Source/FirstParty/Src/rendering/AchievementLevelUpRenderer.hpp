#pragma once

#ifndef ACHIEVEMENT_LEVELUP_RENDERER_HPP
#define ACHIEVEMENT_LEVELUP_RENDERER_HPP

#include "Drawable.hpp"
#include "../gui/AchievementPopUpMenu.hpp"

class AchievementManager;
class Achievement;

class AchievementLevelUpRenderer : public Drawable
{
public:
    AchievementLevelUpRenderer(AchievementManager& achievementManager,
                               ResourceManager& resourceManager,
                               sf::RenderWindow& screen,
                               float drawTime);

    void update();
private:
    void doDraw(const DrawParameter& param) override;

    AchievementManager& m_achievementManager;
    const float m_drawTime;
    sf::Clock m_timer;
    sf::RenderWindow& m_screen;
    bool m_drawing;
    Achievement* m_currentAchievement;
    AchievementPopUpMenu m_menu;
};

#endif // ACHIEVEMENT_LEVELUP_RENDERER_HPP
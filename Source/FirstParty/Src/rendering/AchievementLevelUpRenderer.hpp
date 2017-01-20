#pragma once

#ifndef ACHIEVEMENT_LEVELUP_RENDERER_HPP
#define ACHIEVEMENT_LEVELUP_RENDERER_HPP

#include "Drawable.hpp"


class AchievementManager;
class Achievement;

class AchievementLevelUpRenderer : public Drawable
{
public:
	AchievementLevelUpRenderer(AchievementManager& achievementManager, float drawTime);

	void update();
private:
	void doDraw(const DrawParameter& param) override;

	AchievementManager& m_achievementManager;
	const float m_drawTime;
	sf::Clock m_timer;
	bool m_drawing;
	Achievement* m_currentAchievement;
};

#endif // ACHIEVEMENT_LEVELUP_RENDERER_HPP
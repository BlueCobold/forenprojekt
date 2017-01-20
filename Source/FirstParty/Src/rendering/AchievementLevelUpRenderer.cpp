#include "AchievementLevelUpRenderer.hpp"
#include "../resources/AchievementManager.hpp"
#include "../resources/Achievement.hpp"

AchievementLevelUpRenderer::AchievementLevelUpRenderer(AchievementManager& achievementManager, float drawTime) :
	m_achievementManager(achievementManager),
	m_drawTime(drawTime),
	m_drawing(false)
{ }

void AchievementLevelUpRenderer::doDraw(const DrawParameter& param)
{
	m_currentAchievement->getName();
}

void AchievementLevelUpRenderer::update()
{
	if(m_achievementManager.newAchievementLevelUp() && m_drawing == false)
	{
		m_timer.restart();
		m_drawing = true;
		m_currentAchievement = m_achievementManager.getAchievement(m_achievementManager.popLatestAchievementLevelUp());
		
	}

	if(m_timer.getElapsedTime().asSeconds() > m_drawTime && m_drawing)
	{
		m_drawing = false;
		m_currentAchievement = nullptr;
	}
}
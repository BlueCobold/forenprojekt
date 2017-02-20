#include "AchievementLevelUpRenderer.hpp"
#include "../resources/AchievementManager.hpp"
#include "../resources/Achievement.hpp"
#include "../resources/ResourceManager.hpp"
#include "../gui/LineLabel.hpp"

AchievementLevelUpRenderer::AchievementLevelUpRenderer(AchievementManager& achievementManager,
                                                       ResourceManager& resourceManager,
                                                       sf::RenderWindow& screen,
                                                       float drawTime) :
    m_achievementManager(achievementManager),
    m_drawTime(drawTime),
    m_drawing(false),
    m_screen(screen),
    m_menu(screen, resourceManager)
{ }

void AchievementLevelUpRenderer::doDraw(const DrawParameter& param)
{
    if(m_drawing)
        m_menu.draw(param);
}

void AchievementLevelUpRenderer::update()
{
    if(m_achievementManager.newAchievementLevelUp() && m_drawing == false)
    {
        m_timer.restart();
        m_drawing = true;
        m_currentAchievement = m_achievementManager.getAchievement(m_achievementManager.popLatestAchievementLevelUp());
        m_menu.getLabel(AchievementPopUpMenu::LABEL_ACHIEVEMENT_NAME).setText(m_currentAchievement->getName(), m_currentAchievement->getKey());
    }

    if(m_timer.getElapsedTime().asSeconds() > m_drawTime && m_drawing)
    {
        m_drawing = false;
        m_currentAchievement = nullptr;
    }

    m_menu.update(m_screen, m_timer.getElapsedTime().asSeconds());
}
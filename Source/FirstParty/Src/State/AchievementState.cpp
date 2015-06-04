#include "AchievementState.hpp"

#include "../gui/Button.hpp"
#include "../resources/Config.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include "../MacHelper.hpp"
#include "../resources/AchievementManager.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

#include <fstream> 

AchievementState::AchievementState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 AppConfig& config,
                                 AchievementManager& achievementManager) :
    State(screen, resourceManager, config),
    m_menu(screen, resourceManager),
    m_HUD(resourceManager, config),
    m_achievementManager(achievementManager)
{
    m_currentAchievementIterator = m_achievementManager.beginIterator();
    if(m_currentAchievementIterator != m_achievementManager.endIterator())
    {
        m_menu.getLabel(AchievementMenu::LABEL_ACHIEVEMENT_NAME).setText(m_currentAchievementIterator->second.getId());
        m_menu.getLabel(AchievementMenu::LABEL_ACHIEVEMENT_COUNTER).setText("");
        updateButtons();
    }
}

AchievementState::~AchievementState()
{
}

void AchievementState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation, time);
    
    m_menu.updateLayout();
}

StateChangeInformation AchievementState::update(const float time)
{
    updateTime(time);

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == AchievementMenu::BUTTON_MAIN_MENU)
    {
        m_stateInfo.m_level = nullptr;
        m_stateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        m_transitionStateInfo.m_comeFromeState = AchievementStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    if(clicked == AchievementMenu::BUTTON_RIGHT)
    {
        ++m_currentAchievementIterator;
        updateRightButton();
    }

    if(clicked == AchievementMenu::BUTTON_LEFT)
    {
        --m_currentAchievementIterator;
        updateLeftButton();
    }

    return StateChangeInformation::Empty();
}

void AchievementState::draw(const DrawParameter& params)
{    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}

void AchievementState::updateLeftButton()
{
    if(m_currentAchievementIterator != m_achievementManager.endIterator())
    {
        m_menu.getLabel(AchievementMenu::LABEL_ACHIEVEMENT_NAME).setText(m_currentAchievementIterator->second.getName());
        m_menu.getLabel(AchievementMenu::LABEL_ACHIEVEMENT_COUNTER).setText(utility::toString(m_currentAchievementIterator->second.getCounter()));
        m_menu.hideRightButton(false);

        if(--m_currentAchievementIterator == m_achievementManager.endIterator())
            m_menu.hideLeftButton(true);

        ++m_currentAchievementIterator;
    }
}

void AchievementState::updateRightButton()
{
    if(m_currentAchievementIterator != m_achievementManager.endIterator())
    {
        m_menu.getLabel(AchievementMenu::LABEL_ACHIEVEMENT_NAME).setText(m_currentAchievementIterator->second.getName());
        m_menu.getLabel(AchievementMenu::LABEL_ACHIEVEMENT_COUNTER).setText(utility::toString(m_currentAchievementIterator->second.getCounter()));
        m_menu.hideLeftButton(false);

        if(++m_currentAchievementIterator == m_achievementManager.endIterator())
            m_menu.hideRightButton(true);

        --m_currentAchievementIterator;
    }
}

void AchievementState::updateButtons()
{
    updateRightButton();
    updateLeftButton();
}
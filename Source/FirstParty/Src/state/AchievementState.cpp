#include "AchievementState.hpp"

#include "../gui/Button.hpp"
#include "../gui/InteractiveLabel.hpp"
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
    m_achievementManager(achievementManager),
    m_currentAchievementIndex(0),
    m_transitionStateInfo(AchievementStateId)
{
    auto achievement = m_achievementManager.getAchievement(m_currentAchievementIndex);
    if(achievement)
    {
        updateAchievementData();
        updateButtons();
    }
    m_transitionStateInfo.m_comeFromeState = AchievementStateId;
    m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
}

AchievementState::~AchievementState()
{
}

void AchievementState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    updateAchievementData();
    State::onEnter(enterInformation, time);
}

StateChangeInformation AchievementState::update(const float time)
{
    updateTime(time);

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == AchievementMenu::BUTTON_MAIN_MENU)
    {
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    if(clicked == AchievementMenu::BUTTON_RIGHT)
    {
        ++m_currentAchievementIndex;
        updateRightButton();
        updateAchievementData();
    }

    if(clicked == AchievementMenu::BUTTON_LEFT)
    {
        --m_currentAchievementIndex;
        updateLeftButton();
        updateAchievementData();
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
    auto achievement = m_achievementManager.getAchievement(m_currentAchievementIndex);

    if(achievement)
    {
        m_menu.hideRightButton(false);

        if(m_achievementManager.getAchievement(--m_currentAchievementIndex) == nullptr)
            m_menu.hideLeftButton(true);

        ++m_currentAchievementIndex;
    }
}

void AchievementState::updateRightButton()
{
    auto achievement = m_achievementManager.getAchievement(m_currentAchievementIndex);

    if(achievement)
    {

        m_menu.hideLeftButton(false);

        if(m_achievementManager.getAchievement(++m_currentAchievementIndex) == nullptr)
            m_menu.hideRightButton(true);

        --m_currentAchievementIndex;
    }
}

void AchievementState::updateButtons()
{
    updateRightButton();
    updateLeftButton();
}

void AchievementState::updateAchievementData()
{
    auto achievement = m_achievementManager.getAchievement(m_currentAchievementIndex);

    if(achievement)
    {
        m_menu.getInteractiveLabel(AchievementMenu::LABEL_ACHIEVEMENT_NAME).setText(achievement->getName(m_config.get<std::string>("language")));
        m_menu.getInteractiveLabel(AchievementMenu::LABEL_ACHIEVEMENT_NAME).setToolTipText("tooltip_" + achievement->getKey());
        m_menu.getLabel(AchievementMenu::LABEL_ACHIEVEMENT_COUNTER).setText(utility::toString(achievement->getCounter()));
    }
}

void AchievementState::setLanguage(const std::string& language)
{
    m_menu.setLanguage(language);

    updateAchievementData();
}
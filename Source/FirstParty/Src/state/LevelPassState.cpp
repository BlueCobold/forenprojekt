#include "LevelPassState.hpp"
#include "../Input.hpp"
#include "../model/Level.hpp"
#include "../resources/AppConfig.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include "../gui/InputBox.hpp"
#include "../resources/AchievementManager.hpp"
#ifdef LEVELTESTING
#include "../resources/OpenFileDialoge.hpp"
#endif
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

LevelPassState::LevelPassState(sf::RenderWindow& screen, 
                               ResourceManager& resourceManager, 
                               AppConfig& config,
                               AchievementManager& achievementManager ) :
    State(screen, resourceManager, config),
    m_background(nullptr),
    m_menu(screen, resourceManager),
    m_HUD(resourceManager, config),
    m_replay(false),
    m_gotCoins(false),
    m_achievementManager(achievementManager),
    m_transitionStateInfo(LevelPassStateId)
{
}

LevelPassState::~LevelPassState()
{
}

void LevelPassState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    m_replay = false;
    const EnterPauseStateInformation* info = dynamic_cast<const EnterPauseStateInformation*>(enterInformation);
    m_level = info->m_level;

    State::onEnter(enterInformation, time);
    m_HUD.restartAt(getPassedTime());
    m_HUD.onEnter(m_level);

    m_menu.setPoints(m_level->getPoints());
    m_menu.setGrade(m_level->getMedal());
    m_menu.setLostBalls(m_level->getLostBalls());
    std::string text = utility::replace(utility::replace(utility::replace(utility::translateKey("tooltip_medals"), 
                                        utility::toString(m_level->getMedal(Level::Gold))),   // first replace
                                        utility::toString(m_level->getMedal(Level::Silver))), // second replace 
                                        utility::toString(m_level->getMedal(Level::Bronze))); // third replace
    m_menu.setMedalToolTipText(text);
    if(!enterInformation->m_prepareOnly && !m_gotCoins)
    {
        if(m_level->getPoints() > 0)
            m_config.set<int>("coins", m_config.get<int>("coins") + m_level->getPoints());

        if(m_level->number() == m_config.get<int>("UnlockedLevel"))
            m_config.set<int>("UnlockedLevel", m_level->number() + 1);

        m_gotCoins = true;

        setAchievements();
    }

    m_playStateInfo.m_levelNumber = enterInformation->m_levelNumber;

    m_menu.updateLayout();
}

StateChangeInformation LevelPassState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    updateTime(time);

    m_HUD.update(m_level, getPassedTime());

#ifdef LEVELTESTING
    m_menu.getButton(ReplayMenu::BUTTON_PLAY_NEXT).setVisible(true);
#else
    m_menu.getButton(ReplayMenu::BUTTON_PLAY_NEXT).setVisible(m_playStateInfo.m_levelNumber < m_config.get<int>("UnlockedLevel") &&
                                                              static_cast<unsigned int>(m_playStateInfo.m_levelNumber) <= State::getResourceManager().getFileNames().size());
#endif
    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == ReplayMenu::BUTTON_PLAY_AGAIN)
    {
#ifdef LEVELTESTING
        if(m_level->number() == 0)
        {
            m_loadLevelStateInfo.m_file = m_level->getFileName();
            m_loadLevelStateInfo.m_level = nullptr;
            m_loadLevelStateInfo.m_directPlay = true;
            m_loadLevelStateInfo.m_levelNumber = 0;
        }
        else
        {
            m_loadLevelStateInfo.m_file = "";
            m_loadLevelStateInfo.m_level = m_level;
            m_loadLevelStateInfo.m_directPlay = false;
            m_loadLevelStateInfo.m_levelNumber = m_playStateInfo.m_levelNumber;
        }
        m_loadLevelStateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#else
        m_gotCoins = false;
        m_loadLevelStateInfo.m_prepareOnly = false;
        m_loadLevelStateInfo.m_level = nullptr;
        m_loadLevelStateInfo.m_directPlay = false;
        m_loadLevelStateInfo.m_levelNumber = m_playStateInfo.m_levelNumber;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#endif
    }
    else if(clicked == ReplayMenu::BUTTON_PLAY_NEXT)
    {
#ifdef LEVELTESTING
        bool loadKey = utility::Keyboard.isKeyDown(sf::Keyboard::L) || utility::Keyboard.isKeyPressed(sf::Keyboard::L);
        if(m_level->number() == 0  || loadKey)
        {
            OpenFileDialoge ofd("Level\0*.lvl\0");
            if(ofd.openDialoge())        
                m_loadLevelStateInfo.m_file = ofd.getFile();
            else
                return StateChangeInformation::Empty();

            m_loadLevelStateInfo.m_levelNumber = 0;
            m_loadLevelStateInfo.m_level = nullptr;
            m_loadLevelStateInfo.m_directPlay = true;
            m_loadLevelStateInfo.m_prepareOnly = false;
            m_transitionStateInfo.m_followingState = LoadLevelStateId;
            m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
        }
        else
        {
            m_gotCoins = false;
            m_loadLevelStateInfo.m_level = nullptr;
            m_loadLevelStateInfo.m_prepareOnly = false;
            m_loadLevelStateInfo.m_directPlay = false;
            m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
            if(static_cast<unsigned int>(m_playStateInfo.m_levelNumber) < State::getResourceManager().getFileNames().size())
            {
                m_loadLevelStateInfo.m_levelNumber = m_playStateInfo.m_levelNumber + 1;
                m_transitionStateInfo.m_followingState = LoadLevelStateId;
            }
            else
                m_transitionStateInfo.m_followingState = GameFinishedStateId;

            m_achievementManager.saveValues();
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
        }
#else
        m_gotCoins = false;
        m_loadLevelStateInfo.m_level = nullptr;
        m_loadLevelStateInfo.m_prepareOnly = false;
        m_loadLevelStateInfo.m_directPlay = false;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        if(static_cast<unsigned int>(m_playStateInfo.m_levelNumber) < State::getResourceManager().getFileNames().size())
        {
            m_loadLevelStateInfo.m_levelNumber = m_playStateInfo.m_levelNumber + 1;
            m_transitionStateInfo.m_followingState = LoadLevelStateId;
        }
        else
            m_transitionStateInfo.m_followingState = GameFinishedStateId;

        m_achievementManager.saveValues();
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#endif
    }
    else if(clicked == ReplayMenu::BUTTON_MAIN_MENU)
    {
        m_gotCoins = false;
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = nullptr;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_achievementManager.saveValues();
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == ReplayMenu::BUTTON_HIGHSCORE)
    {
        m_highScoreInfo.m_level = m_level;
        m_highScoreInfo.m_comeFromState = LevelPassStateId;
        m_transitionStateInfo.m_followingState = HighScoreStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_highScoreInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void LevelPassState::draw(const DrawParameter& params)
{
    m_level->adjustView(params);
    m_level->draw(params);

    m_HUD.draw(params);
    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);
    
    m_menu.draw(params);
}

void LevelPassState::setAchievements()
{
    unsigned int stars = 0;
    if(m_level->getMedal() == Level::Gold)
        stars = 3;
    else if(m_level->getMedal() == Level::Silver)
        stars = 2;
    else if(m_level->getMedal() == Level::Bronze)
        stars = 1;

    m_achievementManager.analyseGameEvents(m_level->getGameEvents());

    if(m_level->isTimeAttackMode())
        m_achievementManager.addValueTo(Achievement::Finish, Achievement::InSum, Achievement::LevelTAM, 1);
    else
        m_achievementManager.addValueTo(Achievement::Finish, Achievement::InSum, Achievement::LevelNAM, 1);

    m_achievementManager.addValueTo(Achievement::Finish, Achievement::InSum, Achievement::Level, 1);
    m_achievementManager.addValueTo(Achievement::Collect, Achievement::InSum, Achievement::Coins, m_level->getPoints());
    m_achievementManager.setValueTo(Achievement::Collect, Achievement::InCash, Achievement::Coins, m_config.get<int>("coins"));

    for(unsigned int i = 1; i < AchievementManager::Max_Loop_Counter; ++i)
    {
        if(static_cast<int>(i * AchievementManager::Points_At_Once_Step) <= m_level->getPoints())
            m_achievementManager.addValueTo(Achievement::Collect, Achievement::AtOnce, Achievement::Coins, i * AchievementManager::Points_At_Once_Step, 1);

        if(i <= stars && i <= AchievementManager::Stars_At_Once_Max)
            m_achievementManager.addValueTo(Achievement::Collect, Achievement::AtOnce, Achievement::Stars, i, 1);

        if(i <= AchievementManager::Level_Tam_Min_Of_Time_MaxSteps && m_level->isTimeAttackMode() &&
           (i * AchievementManager::Level_Tam_Min_Of_Time_Step) <= static_cast<unsigned int>(m_level->getLevelPlayTime() * 1000))
            m_achievementManager.addValueTo(Achievement::Finish, Achievement::MinOfTime, Achievement::LevelTAM, i * AchievementManager::Level_Tam_Min_Of_Time_Step, 1);

        if(i <= AchievementManager::Level_Nam_Max_Of_Time_MaxSteps && !m_level->isTimeAttackMode() &&
           (i * AchievementManager::Level_Nam_Max_Of_Time_Step) >= static_cast<unsigned int>(m_level->getLevelPlayTime() * 1000))
            m_achievementManager.addValueTo(Achievement::Finish, Achievement::MaxOfTime, Achievement::LevelNAM, i * AchievementManager::Level_Nam_Max_Of_Time_Step, 1);
    }

    m_achievementManager.addValueTo(Achievement::Loose, Achievement::InSum, Achievement::Ball, m_level->getLostBalls());
    m_achievementManager.addValueTo(Achievement::Collect, Achievement::InSum, Achievement::Stars, stars);
}
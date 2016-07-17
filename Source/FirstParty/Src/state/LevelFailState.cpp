#include "LevelFailState.hpp"
#include "../Input.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include "../model/Level.hpp"
#include "../resources/AchievementManager.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

LevelFailState::LevelFailState(sf::RenderWindow& screen,
                               ResourceManager& resourceManager,
                               AppConfig& config,
                               AchievementManager& achievementManager) :
    State(screen, resourceManager, config),
    m_achievementManager(achievementManager),
    m_background(nullptr),
    m_menu(screen, resourceManager),
    m_HUD(resourceManager, config),
    m_replay(false),
    m_transitionStateInfo(LevelFailStateId)
{
}

LevelFailState::~LevelFailState()
{
}

void LevelFailState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    m_replay = false;
    const EnterPauseStateInformation* info = dynamic_cast<const EnterPauseStateInformation*>(enterInformation);
    m_level = info->m_level;

    State::onEnter(enterInformation, time);
    m_HUD.restartAt(getPassedTime());
    m_HUD.onEnter(m_level);

    m_playStateInfo.m_levelNumber = enterInformation->m_levelNumber;

    if(!enterInformation->m_prepareOnly)
        setAchievements();
}

StateChangeInformation LevelFailState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    updateTime(time);

    m_HUD.update(m_level, getPassedTime());

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == FailMenu::BUTTON_PLAY_AGAIN)
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
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        m_achievementManager.saveValues();
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#else
        m_loadLevelStateInfo.m_level = m_level;
        m_loadLevelStateInfo.m_directPlay = false;
        m_loadLevelStateInfo.m_levelNumber = m_playStateInfo.m_levelNumber;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        m_achievementManager.saveValues();
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#endif
    }
    else if(clicked == FailMenu::BUTTON_MAIN_MENU)
    {
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = nullptr;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_achievementManager.saveValues();
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == FailMenu::BUTTON_HIGHSCORE)
    {
        m_highScoreInfo.m_level = m_level;
        m_highScoreInfo.m_comeFromState = LevelFailStateId;
        m_transitionStateInfo.m_followingState = HighScoreStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_highScoreInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void LevelFailState::draw(const DrawParameter& params)
{
    if(m_level != nullptr)
    {
        m_level->adjustView(params);
        m_level->draw(params);
    }

    m_HUD.draw(params);

    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}

void LevelFailState::setAchievements()
{
    m_achievementManager.analyseGameEvents(m_level->getGameEvents());
    m_achievementManager.addValueTo(Achievement::Loose, Achievement::InSum, Achievement::Ball, m_level->getLostBalls());

    if(m_level->isTimeAttackMode())
        m_achievementManager.addValueTo(Achievement::Loose, Achievement::InSum, Achievement::LevelTAM, 1);
    else
        m_achievementManager.addValueTo(Achievement::Loose, Achievement::InSum, Achievement::LevelNAM, 1);

    m_achievementManager.addValueTo(Achievement::Loose, Achievement::InSum, Achievement::Level, 1);
}

void LevelFailState::setLanguage(const std::string& language)
{
    m_menu.setLanguage(language);
}
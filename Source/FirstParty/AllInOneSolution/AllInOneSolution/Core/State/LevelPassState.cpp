#include "LevelPassState.hpp"
#include "../Input.hpp"
#include "../model/Level.hpp"
#include "../resources/AppConfig.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include "../gui/InputBox.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

LevelPassState::LevelPassState(sf::RenderWindow& screen, 
                               ResourceManager& resourceManager, 
                               AppConfig& config) :
    State(screen, resourceManager, config),
    m_background(nullptr),
    m_menu(screen, resourceManager),
    m_HUD(resourceManager, config),
    m_replay(false),
    m_gotCoins(false)
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

    m_menu.getButton(ReplayMenu::BUTTON_PLAY_NEXT).setVisible(m_playStateInfo.m_levelNumber < m_config.get<int>("UnlockedLevel") &&
                                                              static_cast<unsigned int>(m_playStateInfo.m_levelNumber) <= State::getResourceManager().getFileNames().size());

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == ReplayMenu::BUTTON_PLAY_AGAIN)
    {
        m_gotCoins = false;
        m_loadLevelStateInfo.m_prepareOnly = false;
        m_loadLevelStateInfo.m_level = nullptr;
        m_loadLevelStateInfo.m_directPlay = false;
        m_loadLevelStateInfo.m_levelNumber = m_playStateInfo.m_levelNumber;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        m_transitionStateInfo.m_comeFromeState = LevelPassStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == ReplayMenu::BUTTON_PLAY_NEXT)
    {
        m_gotCoins = false;
        m_loadLevelStateInfo.m_level = nullptr;
        m_loadLevelStateInfo.m_prepareOnly = false;
        m_loadLevelStateInfo.m_directPlay = false;
        m_transitionStateInfo.m_comeFromeState = LevelPassStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        if(static_cast<unsigned int>(m_playStateInfo.m_levelNumber) < State::getResourceManager().getFileNames().size())
        {
            m_loadLevelStateInfo.m_levelNumber = m_playStateInfo.m_levelNumber + 1;
            m_transitionStateInfo.m_followingState = LoadLevelStateId;
        }
        else
            m_transitionStateInfo.m_followingState = GameFinishedStateId;
 
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == ReplayMenu::BUTTON_MAIN_MENU)
    {
        m_gotCoins = false;
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = nullptr;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_transitionStateInfo.m_comeFromeState = LevelPassStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == ReplayMenu::BUTTON_HIGHSCORE)
    {
        m_highScoreInfo.m_level = m_level;
        m_highScoreInfo.m_comeFromState = LevelPassStateId;
        m_transitionStateInfo.m_followingState = HighScoreStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_highScoreInfo;
        m_transitionStateInfo.m_comeFromeState = LevelPassStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void LevelPassState::draw(const DrawParameter& params)
{
    m_level->adjustView(params.getTarget());
    m_level->draw(params);

    m_HUD.draw(params);
    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);
    
    m_menu.draw(params);
}
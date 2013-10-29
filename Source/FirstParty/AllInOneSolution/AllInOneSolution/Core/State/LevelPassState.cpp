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
    m_menu(sf::Vector2f(0, 0), screen, resourceManager),
    m_newHighScoreMenu(sf::Vector2f(0, 0), screen, resourceManager),
    m_HUD(resourceManager, config),
    m_replay(false),
    m_newHighScore(false),
    m_addedHighScore(false)
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

    m_timeDiff = time - info->m_levelTime;
    State::onEnter(enterInformation, time - m_timeDiff);
    m_HUD.restartAt(getCurrentTime());

    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));
    m_menu.setPoints(m_level->getPoints());
    m_menu.setGrade(m_level->getMedal());
    m_menu.setLostBalls(m_level->getLostBalls());
    std::string text = utility::replace(utility::replace(utility::replace(utility::translateKey("tooltip_medals"), 
                                        utility::toString(m_level->getMedal(Level::Gold))),   // first replace
                                        utility::toString(m_level->getMedal(Level::Silver))), // second replace 
                                        utility::toString(m_level->getMedal(Level::Bronze))); // third replace
    m_menu.setMedalToolTipText(text);
    if(!enterInformation->m_prepareOnly && m_level->getPoints() > 0)
        m_config.set<int>("coins", m_config.get<int>("coins") + m_level->getPoints());

    m_playStateInfo.m_levelNumber = enterInformation->m_levelNumber;

    m_newHighScore = checkForNewHighscore();
    if(m_newHighScore && !m_addedHighScore)
        m_newHighScoreMenu.getInputBox(NewHighScoreMenu::INPUTBOX).onEnter();

    m_newHighScoreMenu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));
}

StateChangeInformation LevelPassState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    updateTime(time - m_timeDiff);

    m_HUD.update(m_level, getCurrentTime());

    if(m_newHighScore && !m_addedHighScore)
    {
        if(m_newHighScoreMenu.getInputBox(NewHighScoreMenu::INPUTBOX).isFinished())
            addNewHighScore(m_level->getPoints(), m_newHighScoreMenu.getInputBox(NewHighScoreMenu::INPUTBOX).getText());

        m_newHighScoreMenu.update(m_screen);

        return StateChangeInformation::Empty();
    }
    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    m_menu.getButton(ReplayMenu::BUTTON_PLAY_NEXT).setVisible(m_playStateInfo.m_levelNumber < 7);

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen);

    if(clicked == ReplayMenu::BUTTON_PLAY_AGAIN)
    {
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_addedHighScore = false;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == ReplayMenu::BUTTON_PLAY_NEXT)
    {
        m_stateInfo.m_level = nullptr;
        m_stateInfo.m_prepareOnly = false;
        m_stateInfo.m_levelNumber = m_playStateInfo.m_levelNumber + 1;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        m_addedHighScore = false;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == ReplayMenu::BUTTON_MAIN_MENU)
    {
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = nullptr;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_addedHighScore = false;
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
    m_level->adjustView(params.getTarget());
    m_level->draw(params);

    m_HUD.draw(params);
    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);

    if(m_newHighScore && !m_addedHighScore)
        m_newHighScoreMenu.draw(params);
    else
        m_menu.draw(params);
}

bool LevelPassState::checkForNewHighscore()
{
    int points = m_level->getPoints();
    unsigned int number = m_level->number();
    for(int i = 1; i < 6; ++i)
    {
        std::string term = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i);
        if(points > State::m_config.get<int>(term))
            return true;
    }
    return false;
}
void LevelPassState::addNewHighScore(int points, std::string name)
{
    int newPlace = 0;
    int number = m_level->number();
    for(int i = 5; i > 0; --i)
    {
        std::string term = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i);
        if(points > State::m_config.get<int>(term))
            newPlace = i;
    }

    for(int i = 5; i > newPlace; --i)
    {
        std::string termPoints1 = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i - 1);
        std::string termName1 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i - 1);

        std::string termPoints2 = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i);
        std::string termName2 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i);

        State::m_config.set<int>(termPoints2, State::m_config.get<int>(termPoints1));
        State::m_config.set<std::string>(termName2, State::m_config.get<std::string>(termName1));
    }

    std::string termPoints = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(newPlace);
    std::string termName = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(newPlace);

    State::m_config.set<int>(termPoints, points);
    State::m_config.set<std::string>(termName, name);

    m_addedHighScore = true;
}
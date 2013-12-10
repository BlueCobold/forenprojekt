#include "NewHighScoreState.hpp"
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

NewHighScoreState::NewHighScoreState(sf::RenderWindow& screen, 
                               ResourceManager& resourceManager, 
                               AppConfig& config) :
    State(screen, resourceManager, config),
    m_background(nullptr),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager),
    m_HUD(resourceManager, config),
    m_replay(false)
{
}

NewHighScoreState::~NewHighScoreState()
{
}

void NewHighScoreState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    m_replay = false;
    const EnterPauseStateInformation* info = dynamic_cast<const EnterPauseStateInformation*>(enterInformation);
    m_level = info->m_level;

    m_timeDiff = time - info->m_levelTime;
    State::onEnter(enterInformation, time - m_timeDiff);
    m_HUD.restartAt(getCurrentTime());

    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    m_stateInfo.m_levelNumber = enterInformation->m_levelNumber;

    m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).onEnter();
}

StateChangeInformation NewHighScoreState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    updateTime(time - m_timeDiff);

    m_HUD.update(m_level, getCurrentTime());

    if(m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).isFinished())
    {
        addNewHighScore(m_level->getPoints(), m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).getText());

        m_stateInfo.m_levelNumber = m_level->number();
        m_stateInfo.m_level = m_level;
        m_stateInfo.m_levelTime = m_level->getRemainigTime();
        m_stateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        m_transitionStateInfo.m_followingState = LevelPassStateId;
        m_transitionStateInfo.m_comeFromeState = NewHighScoreStateId;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    m_menu.update(m_screen);

    return StateChangeInformation::Empty();
}

void NewHighScoreState::draw(const DrawParameter& params)
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

void NewHighScoreState::addNewHighScore(int points, std::string name)
{
    if(name == "")
    {
        m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).onEnter();
        return;
    }

    int newPlace = 0;
    int number = m_level->number();
    if(m_level->isTimeAttackMode())
    {
        for(int i = 5; i > 0; --i)
        {
            std::string term = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i) + "TAM";
            if(points > State::m_config.get<int>(term))
                newPlace = i;
        }

        for(int i = 5; i > newPlace; --i)
        {
            std::string termPoints1 = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i - 1) + "TAM";
            std::string termName1 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i - 1) + "TAM";

            std::string termPoints2 = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i) + "TAM";
            std::string termName2 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i) + "TAM";

            State::m_config.set<int>(termPoints2, State::m_config.get<int>(termPoints1));
            State::m_config.set<std::string>(termName2, State::m_config.get<std::string>(termName1));
        }

        std::string termPoints = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(newPlace) + "TAM";
        std::string termName = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(newPlace) + "TAM";
        
        State::m_config.set<int>(termPoints, points);
        State::m_config.set<std::string>(termName, name);
    }

    else
    {
        for(int i = 5; i > 0; --i)
        {
            std::string term = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i) + "NAM";
            if(points > State::m_config.get<int>(term))
                newPlace = i;
        }

        for(int i = 5; i > newPlace; --i)
        {
            std::string termPoints1 = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i - 1) + "NAM";
            std::string termName1 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i - 1) + "NAM";

            std::string termPoints2 = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i) + "NAM";
            std::string termName2 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i) + "NAM";

            State::m_config.set<int>(termPoints2, State::m_config.get<int>(termPoints1));
            State::m_config.set<std::string>(termName2, State::m_config.get<std::string>(termName1));
        }
        std::string termPoints = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(newPlace) + "NAM";
        std::string termName = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(newPlace) + "NAM";

        State::m_config.set<int>(termPoints, points);
        State::m_config.set<std::string>(termName, name);
    }
}
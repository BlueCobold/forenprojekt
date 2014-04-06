#include "LevelFailState.hpp"
#include "../Input.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include "../model/Level.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

LevelFailState::LevelFailState(sf::RenderWindow& screen, 
                               ResourceManager& resourceManager, 
                               AppConfig& config) :
    State(screen, resourceManager, config),
    m_background(nullptr),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager),
    m_HUD(resourceManager, config),
    m_replay(false)
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

    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));
    
    m_playStateInfo.m_levelNumber = enterInformation->m_levelNumber;
}

StateChangeInformation LevelFailState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    updateTime(time);

    m_HUD.update(m_level, getPassedTime());

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == FailMenu::BUTTON_PLAY_AGAIN)
    {
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_transitionStateInfo.m_comeFromeState = LevelFailStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == FailMenu::BUTTON_MAIN_MENU)
    {
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = nullptr;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_transitionStateInfo.m_comeFromeState = LevelFailStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == FailMenu::BUTTON_HIGHSCORE)
    {
        m_highScoreInfo.m_level = m_level;
        m_highScoreInfo.m_comeFromState = LevelFailStateId;
        m_transitionStateInfo.m_followingState = HighScoreStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_highScoreInfo;
        m_transitionStateInfo.m_comeFromeState = LevelFailStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void LevelFailState::draw(const DrawParameter& params)
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

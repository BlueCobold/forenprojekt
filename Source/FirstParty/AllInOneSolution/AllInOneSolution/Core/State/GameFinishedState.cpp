#include "GameFinishedState.hpp"
#include "../Input.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

GameFinishedState::GameFinishedState(sf::RenderWindow& screen, 
                                     ResourceManager& resourceManager, 
                                     AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager)
{
}

GameFinishedState::~GameFinishedState()
{
}

void GameFinishedState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation, time);
    m_menu.updateLayout();
}

StateChangeInformation GameFinishedState::update(const float time)
{
    updateTime(time);

    if(State::isPaused())
    {
        m_menu.update(m_screen, getPassedTime(), MenuElementType::Animation);
        return StateChangeInformation::Empty();
    }

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == GameFinishedMenu::BUTTON_CLOSE)
    {
        m_stateInfo.m_prepareOnly = false;
        m_stateInfo.m_level = nullptr;
        m_stateInfo.m_levelNumber = 1;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        m_transitionStateInfo.m_comeFromeState = GameFinishedStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void GameFinishedState::draw(const DrawParameter& params)
{
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}
#include "StartState.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../rendering/transitions/RandomTransition.hpp"

StartState::StartState(sf::RenderWindow& screen, 
                       ResourceManager& resourceManager, 
                       AppConfig& config) :
    State(screen, resourceManager, config),
    m_transitionStateInfo(StartStateId)
{
}

StartState::~StartState()
{
}

void StartState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation, time);
}

StateChangeInformation StartState::update(const float time)
{
    m_transitionStateInfo.m_followingState = MainMenuStateId;
    m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
    return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
}

void StartState::draw(const DrawParameter& params)
{    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape blackRect;
    blackRect.setSize(m_screen.getView().getSize());
    blackRect.setFillColor(sf::Color(0, 0, 0, 255));
    params.getTarget().draw(blackRect);
}
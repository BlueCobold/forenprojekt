#include "StartState.hpp"
#include "resources/Config.hpp"
#include "resources/ResourceManager.hpp"
#include <SFML/Graphics/RectangleShape.hpp>


StartState::StartState(sf::RenderWindow& screen, 
                       ResourceManager& resourceManager, 
                       Config& config) :
    State(screen, resourceManager, config)
{
}

StartState::~StartState()
{
}

void StartState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation,time);
}

StateChangeInformation StartState::update(const float time)
{
    m_playStateInfo.m_returnFromPause = false;
    m_playStateInfo.m_level = nullptr;
    m_transitionStateInfo.m_followingState = MainMenuStateId;
    m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
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
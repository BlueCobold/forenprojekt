#include "CreditMenuState.hpp"

#include "../gui/Button.hpp"
#include "../resources/Config.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

CreditMenuState::CreditMenuState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager)
{
}

CreditMenuState::~CreditMenuState()
{
}

void CreditMenuState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation, time);

    m_menu.updateLayout();
}

StateChangeInformation CreditMenuState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    updateTime(time);

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == CreditMenu::BUTTON_CLOSE)
    {
        m_stateInfo.m_level = nullptr;
        m_stateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        m_transitionStateInfo.m_comeFromeState = CreditMenuStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void CreditMenuState::draw(const DrawParameter& params)
{    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}
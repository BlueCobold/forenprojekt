#include "CreditMenuState.hpp"

#include "../gui/Button.hpp"
#include "../resources/Config.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

CreditMenuState::CreditMenuState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(screen, resourceManager),
    m_transitionStateInfo(CreditMenuStateId)
{
}

CreditMenuState::~CreditMenuState()
{
}

void CreditMenuState::onEnter(const EnterStateInformation* enterInformation, const double time)
{
    State::onEnter(enterInformation, time);
    m_menu.update(m_screen, getPassedTime());
}

StateChangeInformation CreditMenuState::update(const double time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    updateTime(time);

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == CreditMenu::BUTTON_CLOSE)
    {
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void CreditMenuState::doDraw(const DrawParameter& params)
{    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}

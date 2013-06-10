#include "MainMenuState.hpp"
#include "../Input.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

MainMenuState::MainMenuState(sf::RenderWindow& screen, 
                             ResourceManager& resourceManager, 
                             Config& config) :
    State(screen, resourceManager, config),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager),
    m_background(nullptr),
    m_newGame(false)
{
    m_menu.registerOnClick([this](const Button& sender)
    {
        m_newGame = (sender.getId() == MainMenu::BUTTON_START_NEW_GAME);
    });
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation,time);

    m_newGame = false;

    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));
}

StateChangeInformation MainMenuState::update(const float time)
{
    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    updateTime(time - m_timeDiff);

    m_menu.update(m_screen);

    if(m_newGame)
    {
        m_stateInfo.m_prepareOnly = false;
        m_stateInfo.m_level = nullptr;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    
    return StateChangeInformation::Empty();
}

void MainMenuState::draw(const DrawParameter& params)
{    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}
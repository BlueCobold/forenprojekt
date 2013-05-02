#include "LevelPassState.hpp"
#include "Input.hpp"
#include "resources/Config.hpp"
#include "resources/ResourceManager.hpp"
#include "rendering/transitions/RandomTransition.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

LevelPassState::LevelPassState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
    State(screen, resourceManager, config), m_background(nullptr),
    m_gameMenu(sf::Vector2f(0, 0), "Congratulation!", GameMenu::PlayAgain, screen, resourceManager),
    m_HUD(resourceManager, config)
{
    m_gameMenu.setPosition(sf::Vector2f(screen.getSize().x / 2.f - m_gameMenu.getSize().x / 2.f, screen.getSize().y / 2.f - m_gameMenu.getSize().y / 2.f));
    m_screenSize = m_screen.getSize();
}

LevelPassState::~LevelPassState()
{
}

void LevelPassState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    const EnterPauseStateInformation* info = dynamic_cast<const EnterPauseStateInformation*>(enterInformation);
    m_level = info->m_level;
    
    m_timeDiff = time - info->m_levelTime;
    State::onEnter(enterInformation, time - m_timeDiff);
    m_HUD.restartAt(getCurrentTime());
}

StateChangeInformation LevelPassState::update(const float time)
{
    if(m_screenSize != m_screen.getSize())
    {   
        m_screenSize = m_screen.getSize();
        m_gameMenu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_gameMenu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_gameMenu.getSize().y / 2.f));
    }

    updateTime(time - m_timeDiff);

    m_HUD.update(m_level, getCurrentTime());

    m_gameMenu.update(m_screen);

    if(m_gameMenu.isPressed(GameMenu::PlayAgain))
    {
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
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

    m_gameMenu.draw(params);
}
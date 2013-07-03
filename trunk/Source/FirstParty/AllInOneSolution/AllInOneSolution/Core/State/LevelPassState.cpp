#include "LevelPassState.hpp"
#include "../Input.hpp"
#include "../model/Level.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

LevelPassState::LevelPassState(sf::RenderWindow& screen, 
                               ResourceManager& resourceManager, 
                               Config& config) :
    State(screen, resourceManager, config),
    m_background(nullptr),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager),
    m_HUD(resourceManager, config),
    m_replay(false)
{
    m_menu.registerOnClick([this](const Button& sender)
    {
        m_replay = (sender.getId() == ReplayMenu::BUTTON_PLAY_AGAIN);
    });
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
    m_level->getPoints();

    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));
    m_menu.setPoints(m_level->getPoints());
}

StateChangeInformation LevelPassState::update(const float time)
{
    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    updateTime(time - m_timeDiff);

    m_HUD.update(m_level, getCurrentTime());

    m_menu.update(m_screen);

    if(m_replay)
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

    m_menu.draw(params);
}

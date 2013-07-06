#include "LevelPreviewState.hpp"
#include "../resources/Config.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include "../model/Level.hpp"

LevelPreviewState::LevelPreviewState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 Config& config) :
    State(screen, resourceManager, config),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager),
    m_HUD(resourceManager, config),
    m_level(nullptr),
    m_levelUpdated(false)
{
}

LevelPreviewState::~LevelPreviewState()
{
}

void LevelPreviewState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    m_levelUpdated = false;
    State::onEnter(enterInformation,time);
    m_level = enterInformation->m_level;
    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));
    m_menu.setLevelInfo(m_level->getLevelName());
}

StateChangeInformation LevelPreviewState::update(const float time)
{ 
    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen);

    if(clicked == LevelPreviewMenu::BUTTON_START)
    {
        m_playStateInfo.m_prepareOnly = false;
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = PlayStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void LevelPreviewState::draw(const DrawParameter& params)
{    
    if(m_level != nullptr)
    {
        m_level->adjustView(params.getTarget());
        if(!m_levelUpdated)
        {
            m_level->update(0, params.getTarget());
            m_levelUpdated = true;
            m_HUD.update(m_level, getCurrentTime());
        }
        m_level->draw(params);
        m_HUD.draw(params);
    }

    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}
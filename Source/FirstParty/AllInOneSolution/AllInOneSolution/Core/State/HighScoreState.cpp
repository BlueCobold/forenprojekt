#include "HighScoreState.hpp"
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
#include <string>

HighScoreState::HighScoreState(sf::RenderWindow& screen, 
                               ResourceManager& resourceManager, 
                               AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager)
{
}

HighScoreState::~HighScoreState()
{
}

void HighScoreState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    const EnterHighScoreStateInformation* info = dynamic_cast<const EnterHighScoreStateInformation*>(enterInformation);
    
    m_highScoreStateInfo.m_comeFromState = info->m_comeFromState;
    m_highScoreStateInfo.m_level = info->m_level;
    m_highScoreStateInfo.m_levelNumber = info->m_level->number();

    State::onEnter(info,time);
    loadHighScore(*info->m_level);
    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));
}

StateChangeInformation HighScoreState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    updateTime(time - m_timeDiff);

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen);

    if(clicked == HighScoreMenu::BUTTON_CLOSE)
    { 
        m_stateInfo.m_prepareOnly = true;
        m_stateInfo.m_level = m_highScoreStateInfo.m_level;
        m_stateInfo.m_levelNumber = m_highScoreStateInfo.m_levelNumber;
        m_transitionStateInfo.m_followingState = m_highScoreStateInfo.m_comeFromState;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void HighScoreState::draw(const DrawParameter& params)
{    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}
void HighScoreState::loadHighScore(Level& level)
{
    std::string number = utility::toString(level.number());
    for(int i = 0; i < 5; ++i)
    {
        // read the place data from stash.dat
        m_menu.getLabel(11 + i).setText(State::m_config.get<std::string>("HighScoreLevel" + number + "_Place" + utility::toString(i + 1)));
        // reade the point data from stash.dat
        m_menu.getLabel(21 + i).setText(State::m_config.get<std::string>("HighScoreLevel" + number + "_Points" + utility::toString(i + 1)));
    }
}
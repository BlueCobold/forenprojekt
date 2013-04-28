#include "LevelPassState.hpp"
#include "Input.hpp"
#include "resources/Config.hpp"
#include "resources/ResourceManager.hpp"
#include "rendering\transitions\RandomTransition.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

LevelPassState::LevelPassState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
    State(screen, resourceManager, config), m_background(nullptr),
    m_label("congratulation", sf::Vector2f(100, 50), 0, resourceManager.getBitmapFont("gui_large"), LineLabel::Centered),
    m_playAgain(sf::Vector2f(100,100), "playagain", "buttonIdleFont", resourceManager, screen, "ButtonIdle", sf::Vector2f(0, -5)),
    m_HUD(resourceManager, config)
{
    m_playAgain.bindHover("ButtonHover", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(0, -5));
    
    m_playAgain.bindPressed("ButtonPressed", "buttonHoverFont", sf::Vector2f(0, 0), sf::Vector2f(10, 5));
    
    m_menuBackround.setTexture(*resourceManager.getTexture("Window"));
   
    m_menuBackround.setPosition(screen.getSize().x / 2.f - m_menuBackround.getTextureRect().width / 2.f,
                                screen.getSize().y / 2.f - m_menuBackround.getTextureRect().height / 2.f);

    m_label.setPosition(screen.getSize().x / 2.f, m_menuBackround.getPosition().y + 40);

    m_playAgain.setPosition(sf::Vector2f(screen.getSize().x / 2.f - m_playAgain.getSize().x / 2.f, m_menuBackround.getPosition().y + 100));
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
    updateTime(time - m_timeDiff);

    m_HUD.update(m_level, getCurrentTime());

    m_playAgain.update();

    if(m_playAgain.isPressed())
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
    rebuild();

    m_level->adjustView(params.getTarget());
    m_level->draw(params);

    m_HUD.draw(params);
    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);
    params.getTarget().draw(m_menuBackround);
    m_playAgain.draw(params);
    m_label.draw(params);
}

void LevelPassState::rebuild()
{
    m_menuBackround.setPosition(m_screen.getSize().x / 2.f - m_menuBackround.getTextureRect().width / 2.f,
                                m_screen.getSize().y / 2.f - m_menuBackround.getTextureRect().height / 2.f);

    m_label.setPosition(m_screen.getSize().x / 2.f,
                        m_menuBackround.getPosition().y + 40);

    m_playAgain.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_playAgain.getSize().x / 2.f,
                                         m_menuBackround.getPosition().y + 100));
}
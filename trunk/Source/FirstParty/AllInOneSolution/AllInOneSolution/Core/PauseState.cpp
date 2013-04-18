#include "PauseState.hpp"
#include "Input.hpp"
#include "resources/Config.hpp"
#include "resources/ResourceManager.hpp"
#include "rendering\transitions\GrowingRectangleTransition.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

PauseState::PauseState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
    State(screen, resourceManager, config), m_background(nullptr),
    m_label("game paused", sf::Vector2f(screen.getSize().x / 2.f, screen.getSize().y / 2.f), 0, resourceManager.getBitmapFont("green"), LineLabel::Centered)
{
    m_renderTexture.create(m_screen.getSize().x, m_screen.getSize().y);
}

PauseState::~PauseState()
{
}

void PauseState::onEnter(void *enterInformation)
{
    EnterPauseStateInformation* info = (EnterPauseStateInformation*)enterInformation;
    m_enterPauseTransition = info->m_enterPauseTransition;

    if(!m_enterPauseTransition)
        return;

    m_background = std::move(info->m_background);
    m_level = info->m_level;

    m_renderTexture.clear();
    sf::Sprite background(*m_background);
    background.setPosition(0,0);
    m_renderTexture.draw(background);
    sf::RectangleShape whiteRect;
    whiteRect.setSize(
        sf::Vector2f(
            static_cast<float>(m_background->getSize().x),
            static_cast<float>(m_background->getSize().y)));
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    m_renderTexture.draw(whiteRect);
    m_label.draw(m_renderTexture);
    m_renderTexture.display();

    m_foreground = &m_renderTexture.getTexture();

    m_transitionStateInfo.m_followingState = PauseStateId;
    m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
    // MEMORY LEAK
    // TEMPORARY SOLUTION
    // REPLACE AS SOON AS utility::getRandomTransition() works properly.
    m_transitionStateInfo.m_transition = new GrowingRectangleTransition(m_background.get(), m_foreground, 0.5f); //utility::getRandomTransition(m_background.get(), &m_foreground, 0.5f);
    m_pauseStateInfo.m_enterPauseTransition = false;
}

StateChangeInformation PauseState::update()
{
    if(m_enterPauseTransition)
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);

    if(utility::Keyboard.isKeyDown(sf::Keyboard::P) || utility::Keyboard.isKeyDown(sf::Keyboard::Pause)
        || utility::Keyboard.isKeyDown(sf::Keyboard::Escape))
    {
        m_transitionStateInfo.m_followingState = PlayStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        // MEMORY LEAK
        // TEMPORARY SOLUTION
        // REPLACE AS SOON AS utility::getRandomTransition() works properly.
        m_transitionStateInfo.m_transition = new GrowingRectangleTransition(m_foreground, m_background.get(), 0.5f); //utility::getRandomTransition(&m_foreground, m_background.get(), 0.5f);
        m_playStateInfo.m_level = m_level;
        m_playStateInfo.m_returnFromPause = true;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void PauseState::draw()
{
    sf::Sprite foreground(*m_foreground);
    foreground.setPosition(0, 0);
    m_screen.draw(foreground);
}

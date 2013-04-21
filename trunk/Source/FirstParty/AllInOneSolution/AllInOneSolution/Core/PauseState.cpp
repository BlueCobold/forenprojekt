#include "PauseState.hpp"
#include "Input.hpp"
#include "resources/Config.hpp"
#include "resources/ResourceManager.hpp"
#include "rendering\transitions\RandomTransition.hpp"

#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

PauseState::PauseState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
    State(screen, resourceManager, config), m_background(nullptr),
        m_label(sf::Vector2f(0, 0), 0, resourceManager.getBitmapFont("green"), HUDElement::Center, HUDElement::Center, "game paused", LineLabel::Centered)
{
    // BUG: the PC might not be able to handle this size!
    // we still need a texture as big as possible to handle screen-size-changes during the gameplay
    m_renderTexture.create(1920, 1080);
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
    m_level = std::move(info->m_level);

    renderStateTexture();

    m_transitionStateInfo.m_followingState = PauseStateId;
    m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
    std::unique_ptr<Transition> state = std::unique_ptr<RandomTransition>(
        new RandomTransition(m_background.get(), &m_renderTexture.getTexture(), 0.5f, m_screen.getSize()));
    m_transitionStateInfo.m_transition = std::move(state);
    m_pauseStateInfo.m_enterPauseTransition = false;
}

void PauseState::renderStateTexture() 
{
    auto view = sf::View(
        sf::FloatRect(0, 0,
        static_cast<float>(m_screen.getSize().x),
        static_cast<float>(m_screen.getSize().y)));
    view.setViewport(sf::FloatRect(0, 0,
        view.getSize().x / m_renderTexture.getSize().x,
        view.getSize().y / m_renderTexture.getSize().y));
    m_renderTexture.setView(view);
    m_renderTexture.clear();
    render(m_renderTexture);
    m_renderTexture.display();
};

StateChangeInformation PauseState::update()
{
    if(m_enterPauseTransition)
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);

    if(utility::Keyboard.isKeyDown(sf::Keyboard::P) || utility::Keyboard.isKeyDown(sf::Keyboard::Pause)
        || utility::Keyboard.isKeyDown(sf::Keyboard::Escape))
    {
        renderStateTexture();

        m_transitionStateInfo.m_followingState = PlayStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        std::unique_ptr<Transition> state = std::unique_ptr<RandomTransition>(
            new RandomTransition(&m_renderTexture.getTexture(), m_background.get(), 0.5f, m_screen.getSize()));
        m_transitionStateInfo.m_transition = std::move(state);
        m_playStateInfo.m_level = std::move(m_level);
        m_playStateInfo.m_returnFromPause = true;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    return StateChangeInformation::Empty();
}

void PauseState::draw()
{
    render(m_screen);
}

void PauseState::render(sf::RenderTarget& target)
{
    m_level->adjustView(target);
    m_level->draw(target);

    auto view = sf::View(
        sf::FloatRect(0, 0,
        target.getView().getSize().x,
        target.getView().getSize().y));
    view.setViewport(sf::FloatRect(0, 0,
        view.getSize().x / target.getSize().x,
        view.getSize().y / target.getSize().y));
    target.setView(view);

    sf::RectangleShape whiteRect;
    whiteRect.setSize(
        sf::Vector2f(
        static_cast<float>(target.getView().getSize().x),
        static_cast<float>(target.getView().getSize().y)));
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    target.draw(whiteRect);

    m_label.update(target);
    m_label.draw(target);
}

#include "PlayState.hpp"
#include "Input.hpp"
#include "resources/Config.hpp"
#include "resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

PlayState::PlayState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
    State(screen, resourceManager, config),
    m_level(nullptr),
    m_hud(resourceManager, config)
{
}

PlayState::~PlayState()
{

}

void PlayState::onEnter(void *enterInformation)
{
    EnterPlayStateInformation* info = (EnterPlayStateInformation*)enterInformation;
    m_level = std::move(info->m_level);
    float time = m_frametime.getElapsedTime().asSeconds();
    
    utility::Mouse.capture();
    m_level->adaptToMouse();

    if(info->m_returnFromPause)
        m_timeShift = time - m_timeShift;

    else
    {
        m_level->restartAt(time);
        m_level->update(time, m_screen);
        m_hud.update(m_level.get(), time);
        m_timeShift = 0.0f;
    }
}

StateChangeInformation PlayState::update()
{
    float time = m_frametime.getElapsedTime().asSeconds() - m_timeShift;
    m_level->update(time, m_screen);

    if(utility::Keyboard.isKeyDown(sf::Keyboard::R))
    {
        m_timeShift = 0.0f;
        m_level.reset();
        m_level = std::unique_ptr<Level>(new Level(2, m_resourceManager, m_config));
        float time = m_frametime.getElapsedTime().asSeconds();
        m_level->restartAt(time);
    }

    if(utility::Keyboard.isKeyDown(sf::Keyboard::P) || utility::Keyboard.isKeyDown(sf::Keyboard::Pause))
    {
        m_timeShift = time;
        auto source = std::unique_ptr<sf::Texture>(new sf::Texture);
        source->create(m_screen.getSize().x, m_screen.getSize().y);
        // BUG: updating the texture with the background is incorrect. The level+hud should be rendered into it.
        // This bug causes a tiny movement when the state starts - hard to see, but if you update step by step,
        // it gets visible. The further the ball moved in last level->update(), the bigger this movement is.
        source->update(m_screen);
        m_pauseStateInfo.m_background = std::move(source);
        m_pauseStateInfo.m_level = std::move(m_level);
        m_pauseStateInfo.m_enterPauseTransition = true;
        return StateChangeInformation(PauseStateId, &m_pauseStateInfo);
    }

    m_hud.update(m_level.get(), time);
    return StateChangeInformation::Empty();
}

void PlayState::draw()
{
    m_level->draw(m_screen);
    m_hud.draw(m_screen);
}

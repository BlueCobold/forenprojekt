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
    m_level = std::unique_ptr<Level>((Level*)enterInformation);
    float time = m_frametime.getElapsedTime().asSeconds();
    m_level->restartAt(time);
    m_level->update(time, m_screen);
    if(m_level->getRemainingBall() > 0)
        m_hud.setBallShow(true);
    m_hud.update(m_level.get(), m_frametime.getElapsedTime().asSeconds());
}

StateChangeInformation PlayState::update()
{
    m_level->update(m_frametime.getElapsedTime().asSeconds(), m_screen);

    if(utility::Keyboard.isKeyDown(sf::Keyboard::R))
    {
        m_level.reset();
        m_level = std::unique_ptr<Level>(new Level(2, m_resourceManager, m_config));
        float time = m_frametime.getElapsedTime().asSeconds();
        m_level->restartAt(time);
    }

    m_hud.update(m_level.get(),m_frametime.getElapsedTime().asSeconds());
    return StateChangeInformation::Empty();
}

void PlayState::draw()
{
    m_level->draw(m_screen);
    m_hud.draw(m_screen);
}

#include "PlayState.hpp"
#include "resources/ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Config.hpp"

PlayState::PlayState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
    State(screen, resourceManager, config),
    m_level(nullptr),
    m_hud(screen, resourceManager, config)
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
}

StateChangeInformation PlayState::update()
{
    m_level->update(m_frametime.getElapsedTime().asSeconds(), m_screen);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        m_level.reset();
        m_level = std::unique_ptr<Level>(new Level(2, m_resourceManager, m_config));
        float time = m_frametime.getElapsedTime().asSeconds();
        m_level->restartAt(time);
    }

    m_hud.update(m_level.get());
    return StateChangeInformation::Empty();
}

void PlayState::draw()
{
    m_level->draw(m_screen);
    m_hud.draw();
}

#include "PlayState.hpp"
#include "ResourceManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

PlayState::PlayState(sf::RenderWindow& screen, ResourceManager& resourceManager) :
    State(screen, resourceManager),
    m_level(nullptr) // Make sure
{
    screen.setFramerateLimit(60);
    m_level = std::unique_ptr<Level>(new Level(1, m_resourceManager));
}

PlayState::~PlayState()
{

}

void PlayState::update()
{
    m_level->update(m_frametime.getElapsedTime().asSeconds());
}

void PlayState::draw()
{
    m_level->draw(m_screen);
}

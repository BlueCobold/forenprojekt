#include "TestState.hpp"

#include "StateManager.hpp"
#include "ResourceManager.hpp"
#include "PlayState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

TestState::TestState(StateManager& stateManager, sf::RenderWindow& screen, ResourceManager& resourceManager) :
    State(screen, resourceManager),
    m_stateManager(stateManager)
{
    m_texture.loadFromFile("res/img/sprite.png");
    m_clock.restart();
    
    m_animation.bindAnimation(true, 0, 0.1f, 8, 64, 64);
    m_animation.bindTexture(m_texture);
    
    m_animation.setPosition(0, 64);

    m_animation.getAnimation().start();
}

TestState::~TestState()
{

}

void TestState::update()
{
    float x = m_animation.getPosition().x, y = m_animation.getPosition().y;

    m_animation.update( m_clock.getElapsedTime().asSeconds());

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_animation.setPosition(x - 0.1f, y);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_animation.setPosition(x + 0.1f, y);
    }

    // Switch state
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        m_stateManager.push(std::unique_ptr<PlayState>(new PlayState(m_screen, m_resourceManager)));
        m_stateManager.next();
    }
}

void TestState::draw()
{
    m_screen.draw(m_animation);
}

#include "TestState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

TestState::TestState(sf::RenderWindow& screen) :
    State(screen)
{
}

TestState::~TestState()
{
}

bool TestState::initialize()
{
    m_texture.loadFromFile("res/img/sprite.png");
    m_clock.restart();
    
    m_animation.bindAnimation(true, 0, 0.1f, 8, 64, 64);
    m_animation.bindTexture(m_texture);
    
    m_animation.setPosition(0, 64);

    m_animation.getAnimation().start();

    return true;
}

void TestState::update()
{
    float x = m_animation.getPosition().x, y = m_animation.getPosition().y;

    m_animation.update( m_clock.getElapsedTime().asSeconds());

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_animation.setPosition(x - 0.1, y);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_animation.setPosition(x + 0.1, y);
    }

}

void TestState::draw()
{
    m_screen.draw(m_animation);
}
    
void TestState::shutdown()
{
}

#include "TestState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

TestState::TestState(sf::RenderWindow& screen) :
    State(screen),
    m_animation(true, 0, 1, 0.1f, 8, 64, 64)
{
}

TestState::~TestState()
{
}

bool TestState::initialize()
{
    m_texture.loadFromFile("sprite.png");
    m_clock.restart();
    m_animation.bindTexture(m_texture);
    m_animation.start();

    return true;
}

void TestState::update()
{
    m_animation.update( m_clock.getElapsedTime().asSeconds());
}

void TestState::draw()
{
    m_screen.draw(m_animation);
}
    
void TestState::shutdown()
{
}

#include "Test.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

Test::Test(sf::RenderWindow& screen) :
    State(screen),
    m_animation(true, 0, 1, 0.1, 8, 64, 64)
{
}

Test::~Test()
{
}

bool Test::initialize()
{
    m_texture.loadFromFile("sprite.png");
    m_clock.restart();
    m_animation.bindTexture(m_texture);
    m_animation.start();

    return true;
}

void Test::update()
{
    m_animation.update( m_clock.getElapsedTime().asSeconds());
}

void Test::draw()
{
    m_screen.draw(m_animation);
}
    
void Test::shutdown()
{
}

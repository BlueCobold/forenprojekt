#include "TestState2.hpp"


TestState2::TestState2(sf::RenderWindow& screen) : State(screen)
{
}


TestState2::~TestState2()
{
}

bool TestState2::initialize()
{
    m_circle.setFillColor(sf::Color::Red);
    m_circle.setPosition(State::m_screen.getSize().x / 2, State::m_screen.getSize().y / 2);
    m_circle.setRadius(100);
    return true;
}

void TestState2::update()
{
}

void TestState2::draw()
{
    State::m_screen.draw(m_circle);
}

void TestState2::shutdown()
{
}

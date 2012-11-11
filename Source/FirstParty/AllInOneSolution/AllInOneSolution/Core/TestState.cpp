#include "TestState.hpp"


TestState::TestState(sf::RenderWindow& screen) : State(screen)
{
}


TestState::~TestState()
{
}

bool TestState::initialize()
{
    m_circle.setFillColor(sf::Color::Blue);
    m_circle.setPosition(State::m_screen.getSize().x / 2, State::m_screen.getSize().y / 2);
    m_circle.setRadius(100);
    return true;
}

void TestState::update()
{
}

void TestState::draw()
{
    State::m_screen.draw(m_circle);
}

void TestState::shutdown()
{
}

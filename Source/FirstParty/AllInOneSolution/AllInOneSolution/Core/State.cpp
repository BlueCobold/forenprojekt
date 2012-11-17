#include "State.hpp"

State::State(sf::RenderWindow& screen) :
    m_screen(screen),
    m_pause(false)
{

}

State::~State()
{

}

void State::pause()
{
    m_pause = true;
}

void State::resume()
{
    m_pause = false;
}

const bool State::isPaused()
{
    return  m_pause;
}

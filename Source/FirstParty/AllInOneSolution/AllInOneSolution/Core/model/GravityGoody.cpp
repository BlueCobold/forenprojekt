#include "GravityGoody.hpp"

GravityGoody::GravityGoody(const sf::Keyboard::Key key,
                           const float gravityFactor,
                           b2Vec2& gravity) :
    Goody(key),
    m_gravityFactor(gravityFactor),
    m_currentGravity(gravity)
{
}

void GravityGoody::update(const float elapsedTime)
{
    Goody::update(elapsedTime);
    if(isActive())
        m_currentFactor = m_gravityFactor;
    else
        m_currentFactor = 1;
}

b2Vec2 GravityGoody::getGravity() const
{
    return b2Vec2(m_currentGravity.x * m_currentFactor, m_currentGravity.y * m_currentFactor);
}

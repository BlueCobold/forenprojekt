#include "GravityGoodie.hpp"

GravityGoodie::GravityGoodie(const sf::Keyboard::Key key,
                             const float gravityFactor,
                             b2Vec2& gravity) :
    Goodie(key),
    m_gravityFactor(gravityFactor),
    m_currentGravity(gravity)
{
}

void GravityGoodie::update(const float elapsedTime)
{
    Goodie::update(elapsedTime);
    if(Goodie::isActive())
        m_gravityFactor = 2;
    else
        m_gravityFactor = 1;
}

b2Vec2 GravityGoodie::getGravity() const
{
    return b2Vec2(m_currentGravity.x * m_gravityFactor, m_currentGravity.y * m_gravityFactor);
}

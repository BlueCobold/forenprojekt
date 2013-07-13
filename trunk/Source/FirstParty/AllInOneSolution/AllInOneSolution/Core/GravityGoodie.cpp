#include "GravityGoodie.hpp"

GravityGoodie::GravityGoodie(const sf::Keyboard::Key key,
                             const float gravityFactor) :
    Goodie(key),
    m_gravityFactor(gravityFactor),
    m_normalGravity(b2Vec2(0, 9.81f)),
    m_currentGravity(b2Vec2(0, 9.81f))
{
}

void GravityGoodie::update(const float elapsedTime)
{
    Goodie::update(elapsedTime);
    if(Goodie::isActive())
        m_currentGravity = b2Vec2(m_normalGravity.x * m_gravityFactor, m_normalGravity.y * m_gravityFactor);
    else
        m_currentGravity = m_normalGravity;
}

void GravityGoodie::setNormalGravity(const b2Vec2& normalGravity)
{
    m_normalGravity = normalGravity;
}

b2Vec2 GravityGoodie::getGravity()
{
    return m_currentGravity;
}
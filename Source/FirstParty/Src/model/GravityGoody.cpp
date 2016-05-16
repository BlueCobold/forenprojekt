#include "GravityGoody.hpp"

GravityGoody::GravityGoody(const sf::Keyboard::Key key,
                           const float gravityFactor,
                           b2Vec2& gravity,
                           const int charge) :
#ifdef TOUCHSIM
    Goody(key, Goody::GravityGoody, sf::FloatRect(0.f, 50.f, 56.f, 64.f), 0, 0, charge),
#else
    Goody(key, Goody::GravityGoody, 0, 0, charge),
#endif
    m_gravityFactor(gravityFactor),
    m_activeStartTime(0),
    m_activeTime(0),
    m_currentGravity(gravity)
{
}

void GravityGoody::update(const float elapsedTime)
{
    Goody::update(elapsedTime);
    if(isActive())
    {
        m_currentFactor = m_gravityFactor;
        if(m_activeStartTime == 0)
            m_activeStartTime = elapsedTime;
        else
            m_activeTime = elapsedTime - m_activeStartTime;
    }
    else
    {
        m_currentFactor = 1;
        m_activeTime = 0;
        m_activeStartTime = 0;
    }
}

b2Vec2 GravityGoody::getGravity() const
{
    return b2Vec2(m_currentGravity.x * m_currentFactor, m_currentGravity.y * m_currentFactor);
}

const int GravityGoody::getBonusPoints() const
{
    if(m_activeTime >= 15.f)
        return 20;
    else if(m_activeTime >= 10.f)
        return 10;
    else if(m_activeTime >= 5.f)
        return 5;
    else
        return 0;
}

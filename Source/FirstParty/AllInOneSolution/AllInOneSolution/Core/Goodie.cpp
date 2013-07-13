#include "Goodie.hpp"

Goodie::Goodie(const sf::Keyboard::Key key,
               const float durationTime,
               const float cooldownTime, 
               const int charges) :
    m_key(key),
    m_durationTime(durationTime),
    m_cooldownTime(cooldownTime),
    m_charges(charges),
    m_nextUseTime(0),
    m_durationUntilTime(0)
{
}

void Goodie::update(const float elapsedTime)
{
    if(utility::Keyboard.isKeyDown(m_key) && m_charges != 0 && m_nextUseTime < elapsedTime)
    {
        m_active = true;
        if(m_charges > 0)
            m_charges--;
        if(m_cooldownTime > 0)
            m_nextUseTime = elapsedTime + m_cooldownTime;
        if(m_durationTime > 0)
            m_durationUntilTime = elapsedTime + m_durationTime;
    }
    else if(utility::Keyboard.isKeyPressed(m_key) && m_charges != 0 && m_nextUseTime < elapsedTime)
        m_active = true;
    else if(m_durationUntilTime > elapsedTime)
        m_active = true;
    else
        m_active = false;
}

const bool Goodie::isActive() const
{
    return m_active;
}
#include "Goody.hpp"

Goody::Goody(const sf::Keyboard::Key key,
             const Type type,
             const float durationTime,
             const float cooldownTime, 
             const int charges) :
    m_key(key),
    m_durationTime(durationTime),
    m_cooldownTime(cooldownTime),
    m_charges(charges),
    m_nextUseTime(0),
    m_durationUntilTime(0),
    m_selected(false),
    m_type(type),
    m_callback(nullptr),
    m_active(false)
{
}

void Goody::update(const float elapsedTime)
{
    if((utility::Keyboard.isKeyDown(m_key) || (m_selected && utility::Mouse.leftButtonDown()))
       && m_charges != 0 && m_nextUseTime < elapsedTime)
    {
        if(canActivate())
            m_active = true;
        if(m_charges > 0)
            m_charges--;
        if(m_cooldownTime > 0)
            m_nextUseTime = elapsedTime + m_cooldownTime;
        if(m_durationTime > 0)
            m_durationUntilTime = elapsedTime + m_durationTime;

        if(m_callback != nullptr)
            m_callback(*this);
    }
    else if((utility::Keyboard.isKeyPressed(m_key) || (m_selected && utility::Mouse.leftButtonPressed()))
            && m_charges == -1 && m_nextUseTime < elapsedTime)
    {
        if(canActivate())
            m_active = true;
        
        if(m_callback != nullptr)
            m_callback(*this);
    }
    else if(m_durationUntilTime < elapsedTime)
        m_active = false;
}

bool Goody::isActive() const
{
    return m_active;
}

bool Goody::isSelected() const
{
    return m_selected;
}

bool Goody::canActivate() const
{
    return true;
}

int Goody::getCharges() const
{
    return m_charges;
}

void Goody::setSelected(const bool selected)
{
    m_selected = selected;
}

const Goody::Type Goody::getType() const
{
    return m_type;
}

void Goody::registerForActivation(std::function<void(Goody& sender)> callback)
{
    m_callback = callback;
}

void Goody::setCharges(const int charges)
{
    m_charges = charges;
}
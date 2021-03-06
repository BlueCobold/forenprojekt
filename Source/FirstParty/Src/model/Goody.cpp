#include "Goody.hpp"

#ifdef TOUCHSIM
Goody::Goody(sf::Keyboard::Key key,
             Type type,
             const sf::FloatRect& touchArea,
             double durationTime,
             double cooldownTime,
             int charges) :
    m_button(touchArea),
    m_key(key),
    m_active(false),
    m_charges(charges),
    m_cooldownTime(cooldownTime),
    m_nextUseTime(0),
    m_durationTime(durationTime),
    m_durationUntilTime(0),
    m_selected(false),
    m_type(type),
    m_callback(nullptr)
{
}
#else
Goody::Goody(sf::Keyboard::Key key,
             Type type,
             double durationTime,
             double cooldownTime,
             int charges) :
    m_key(key),
    m_active(false),
    m_charges(charges),
    m_cooldownTime(cooldownTime),
    m_nextUseTime(0),
    m_durationTime(durationTime),
    m_durationUntilTime(0),
    m_selected(false),
    m_type(type),
    m_callback(nullptr)
{
}
#endif

double Goody::getCurrentTime()
{
    return m_currentTime;
}

void Goody::update(const double elapsedTime)
{
    m_currentTime = elapsedTime;
#ifdef TOUCHSIM
    m_button.update();
    bool down = utility::Keyboard.isKeyDown(m_key) || (m_selected && utility::Mouse.leftButtonDown()) || m_button.isDown();
    bool pressed = utility::Keyboard.isKeyPressed(m_key) || (m_selected && utility::Mouse.leftButtonPressed()) || m_button.isPressed();
#else
    bool down = utility::Keyboard.isKeyDown(m_key) || (m_selected && utility::Mouse.leftButtonDown());
    bool pressed = utility::Keyboard.isKeyPressed(m_key) || (m_selected && utility::Mouse.leftButtonPressed());
#endif
    if(down && m_charges != 0 && m_nextUseTime < elapsedTime)
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
    else if(pressed && m_charges == -1 && m_nextUseTime < elapsedTime)
    {
        if(canActivate())
            m_active = true;

        if(m_callback != nullptr)
            m_callback(*this);
    }
    else if(m_durationUntilTime < elapsedTime)
        m_active = false;

    updated();
}

void Goody::updated()
{ }

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
    return calculateCanActivate();
}

bool Goody::calculateCanActivate() const
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
